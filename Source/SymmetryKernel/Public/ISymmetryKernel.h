// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.
#pragma once

#include <cstdint>
#include <string> // Explicitly include for std::string
#include <any>    // Explicitly include for std::any

/**
 * @brief Defines the 2-Bit State Matrix for the Symmetry Kernel.
 */
enum class ESymmetryState : uint8_t
{
    Null = 0,
    Protocol = 1,
    Latency = 2,
    Equilibrium = 3
};

/**
 * @class ISymmetryKernel
 * @brief Pure C++ interface for the Wisp Symmetry Kernel, the "Brain" of the AGI.
 */
class ISymmetryKernel
{
public:
    virtual ~ISymmetryKernel() = default;

    /**
     * @brief Processes a signal, the primary entry point for all sensory data.
     */
    virtual void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation) = 0;

    /**
     * @brief Provides the "Parental Truth" for a signal currently in the Protocol (0,1) state.
     * @param InSynthesizedTag A const reference to a std::string. This is the corrected signature.
     */
    virtual void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag) = 0;

    /**
     * @brief Sets the weight of the parental authority, controlling the rate of trust decay.
     */
    virtual void SetParentalAuthorityWeight(float InWeight) = 0;
};
