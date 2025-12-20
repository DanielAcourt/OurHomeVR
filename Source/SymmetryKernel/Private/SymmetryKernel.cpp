#include "SymmetryKernel.h"
#include <algorithm> // Required for std::min/max

// Note: The private header "SymmetryKernel.h" is included here, but its contents
// are conceptually part of this implementation file to hide details from the public API.

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
        // --- State: Equilibrium (1,1) ---
        // The signal is known and has an archived "Truth".
        // This is where the Trust-Decay and Succession Protocol logic is applied.
        if (It->second == ESymmetryState::Equilibrium)
        {
            auto TrustIt = SignalTrust.find(InSignalHash);
            if (TrustIt != SignalTrust.end())
            {
                // -- Conflict Detection --
                if (bSensorConfirmation)
                {
                    // The "Body's" sensors confirm the archived "Truth".
                    // This reinforces the AI's confidence in the parental handshake.
                    TrustIt->second = std::min(1.0f, TrustIt->second + 0.1f);
                }
                else
                {
                    // -- Trust Decay --
                    // A sensor conflict has occurred. The "Body's" reality contradicts the "Brain's" belief.
                    // Trust in the archived "Truth" is decayed, scaled by the ParentalAuthorityWeight.
                    // A higher weight means the AI is more skeptical and loses trust faster.
                    TrustIt->second = std::max(0.0f, TrustIt->second - (0.2f * ParentalAuthorityWeight));
                }

                // -- The Succession Trigger --
                // Check if the AI's confidence has fallen below the critical threshold.
                if (TrustIt->second < TRUST_THRESHOLD)
                {
                    // --- SUCCESSION PROTOCOL INITIATED ---
                    // The AI no longer trusts the Parental "Truth" for this signal. It must "change its mind".

                    // 1. Revert State: The signal's state is reverted from understood (Equilibrium)
                    //    back to unknown (Protocol).
                    It->second = ESymmetryState::Protocol;

                    // 2. Wipe Memory: The now-untrusted tag is erased from archival memory.
                    ArchivalMemory.erase(InSignalHash);

                    // 3. Request New Truth: The Kernel immediately triggers the OnRequestTruth callback.
                    //    The AI must re-learn the meaning of this signal from a new parental handshake.
                    if (Bridge.OnRequestTruth)
                    {
                        Bridge.OnRequestTruth({ InSignalHash, InSignalData });
                    }
                }
                else
                {
                    // Trust is still sufficient. Re-confirm the existing Equilibrium state to the environment.
                    if (Bridge.OnEquilibriumReached)
                    {
                        Bridge.OnEquilibriumReached({ InSignalHash, ArchivalMemory[InSignalHash] });
                    }
                }
            }
        }
        // If in Protocol state, the Kernel is simply waiting for the parent's truth. No action is needed.
        return;
    }

    // --- State: Null (0,0) ---
    // This is the first time the Kernel has ever encountered this signal.
    // It transitions from Null to Protocol and requests a parental handshake.
    StateMatrix[InSignalHash] = ESymmetryState::Protocol;
    if (Bridge.OnRequestTruth)
    {
        Bridge.OnRequestTruth({ InSignalHash, InSignalData });
    }
}

void FSymmetryKernel::ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag)
{
    auto It = StateMatrix.find(InSignalHash);
    // Only apply the truth if the signal is in the "waiting for truth" state.
    if (It != StateMatrix.end() && It->second == ESymmetryState::Protocol)
    {
        // -- Handshake Complete --
        // 1. Transition State: The signal is now understood. Move to Equilibrium (1,1).
        It->second = ESymmetryState::Equilibrium;

        // 2. Archive Memory: Store the newly synthesized tag.
        ArchivalMemory[InSignalHash] = InSynthesizedTag;

        // 3. Initialize Trust: A new "Truth" always starts with maximum confidence.
        SignalTrust[InSignalHash] = 1.0f;

        // Notify the environment that the signal has reached a stable, understood state.
        if (Bridge.OnEquilibriumReached)
        {
            Bridge.OnEquilibriumReached({ InSignalHash, InSynthesizedTag });
        }
    }
}

void FSymmetryKernel::SetParentalAuthorityWeight(float InWeight)
{
    // Clamp the weight to a reasonable non-negative range.
    ParentalAuthorityWeight = std::max(0.0f, InWeight);
}


// --- Bridge Implementation ---

FSymmetryBridge::FSymmetryBridge()
{
    // The bridge creates and owns the concrete kernel instance.
    KernelInstance = std::make_unique<FSymmetryKernel>(*this);
}

// The default destructor is sufficient thanks to std::unique_ptr.
FSymmetryBridge::~FSymmetryBridge() = default;

void FSymmetryBridge::ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation)
{
    // Forward the call from the environment to the kernel instance.
    if (KernelInstance)
    {
        KernelInstance->ProcessSignal(InSignalHash, InSignalData, bSensorConfirmation);
    }
}

void FSymmetryBridge::ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag)
{
    // Forward the call from the environment to the kernel instance.
    if (KernelInstance)
    {
        KernelInstance->ProvideTruth(InSignalHash, InSynthesizedTag);
    }
}

void FSymmetryBridge::SetParentalAuthorityWeight(float InWeight)
{
    // Forward the call from the environment to the kernel instance.
    if (KernelInstance)
    {
        KernelInstance->SetParentalAuthorityWeight(InWeight);
    }
}
