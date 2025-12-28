// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.
#include "SymmetryKernel.h"
#include <algorithm> // Explicitly include for std::min and std::max
#include <iostream>  // For std::cout in the bridge implementation (if any)

// --- Kernel Implementation ---

FSymmetryKernel::FSymmetryKernel(FSymmetryBridge& InBridge)
    : Bridge(InBridge)
{
}

void FSymmetryKernel::ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation)
{
    auto It = StateMatrix.find(InSignalHash);
    if (It != StateMatrix.end())
    {
        if (It->second == ESymmetryState::Equilibrium)
        {
            auto TrustIt = SignalTrust.find(InSignalHash);
            if (TrustIt != SignalTrust.end())
            {
                if (bSensorConfirmation)
                {
                    TrustIt->second = std::min(1.0f, TrustIt->second + 0.1f);
                }
                else
                {
                    TrustIt->second = std::max(0.0f, TrustIt->second - (0.2f * ParentalAuthorityWeight));
                }

                if (TrustIt->second < TRUST_THRESHOLD)
                {
                    It->second = ESymmetryState::Protocol;
                    ArchivalMemory.erase(InSignalHash);
                    if (Bridge.OnRequestTruth)
                    {
                        Bridge.OnRequestTruth({ InSignalHash, InSignalData });
                    }
                }
                else
                {
                    if (Bridge.OnEquilibriumReached)
                    {
                        Bridge.OnEquilibriumReached({ InSignalHash, ArchivalMemory[InSignalHash] });
                    }
                }
            }
        }
        return;
    }

    StateMatrix[InSignalHash] = ESymmetryState::Protocol;
    if (Bridge.OnRequestTruth)
    {
        Bridge.OnRequestTruth({ InSignalHash, InSignalData });
    }
}

// This is the corrected function definition, matching the header signatures.
void FSymmetryKernel::ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag)
{
    auto It = StateMatrix.find(InSignalHash);
    if (It != StateMatrix.end() && It->second == ESymmetryState::Protocol)
    {
        It->second = ESymmetryState::Equilibrium;
        ArchivalMemory[InSignalHash] = InSynthesizedTag;
        SignalTrust[InSignalHash] = 1.0f;

        if (Bridge.OnEquilibriumReached)
        {
            Bridge.OnEquilibriumReached({ InSignalHash, InSynthesizedTag });
        }
    }
}

void FSymmetryKernel::SetParentalAuthorityWeight(float InWeight)
{
    ParentalAuthorityWeight = std::max(0.0f, InWeight);
}


// --- Bridge Implementation ---

FSymmetryBridge::FSymmetryBridge()
{
    KernelInstance = std::make_unique<FSymmetryKernel>(*this);
}

FSymmetryBridge::~FSymmetryBridge() = default;

void FSymmetryBridge::ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation)
{
    if (KernelInstance)
    {
        KernelInstance->ProcessSignal(InSignalHash, InSignalData, bSensorConfirmation);
    }
}

void FSymmetryBridge::ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag)
{
    if (KernelInstance)
    {
        KernelInstance->ProvideTruth(InSignalHash, InSynthesizedTag);
    }
}

void FSymmetryBridge::SetParentalAuthorityWeight(float InWeight)
{
    if (KernelInstance)
    {
        KernelInstance->SetParentalAuthorityWeight(InWeight);
    }
}
