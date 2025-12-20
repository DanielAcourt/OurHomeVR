#pragma once

#include "ISymmetryKernel.h"
#include "SymmetryBridge.h"
#include <unordered_map>
#include <string>
#include <algorithm> // For std::max/min

/**
 * @class FSymmetryKernel
 * @brief Concrete implementation of the ISymmetryKernel interface.
 * @note This header is private to the SymmetryKernel module and should not be included
 * by any external code. Use the public ISymmetryKernel.h and FSymmetryBridge.h for interaction.
 *
 * This class contains the core logic for the zero-knowledge discovery process. It manages
 * the internal state of all observed signals and communicates with the outside world
 * exclusively through the FSymmetryBridge reference provided during construction.
 */
class FSymmetryKernel : public ISymmetryKernel
{
public:
    /**
     * @brief Constructor that requires a reference to the bridge for communication.
     * @param InBridge The communication bridge to the environment.
     */
    explicit FSymmetryKernel(FSymmetryBridge& InBridge);
    virtual ~FSymmetryKernel() override = default;

    //~ Begin ISymmetryKernel Interface
    virtual void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation) override;
    virtual void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag) override;
    virtual void SetParentalAuthorityWeight(float InWeight) override;
    //~ End ISymmetryKernel Interface

private:
    // --- Core State & Memory ---

    /**
     * @brief A map holding the current ESymmetryState of every signal hash the kernel is aware of.
     * This is the "Source of Truth" for the AI's current understanding of its world.
     */
    std::unordered_map<uint64_t, ESymmetryState> StateMatrix;

    /**
     * @brief A map that stores the final, synthesized tag for signals that have reached Equilibrium.
     * This represents the AI's long-term memory of what a signal "means".
     */
    std::unordered_map<uint64_t, std::string> ArchivalMemory;

    // --- Trust & Authority ---

    /**
     * @brief Tracks the confidence (0.0 to 1.0) for each signal in an Equilibrium state.
     * This is the core of the "Relative Truth" mechanic, allowing the AI to question its own beliefs.
     */
    std::unordered_map<uint64_t, float> SignalTrust;

    /**
     * @brief The weight used to scale trust decay during a sensor conflict. Higher values mean faster decay.
     * This is set by the environment via SetParentalAuthorityWeight().
     */
    float ParentalAuthorityWeight = 1.0f;

    /**
     * @brief The trust threshold below which a signal's truth is questioned, triggering the Succession Protocol.
     * If confidence drops below this value, the AI will revert the signal to a Protocol state and request a new handshake.
     */
    const float TRUST_THRESHOLD = 0.2f;

    /**
     * @brief A reference to the communication bridge.
     * This is the *only* way the Kernel can communicate with the outside world (e.g., to fire callbacks).
     */
    FSymmetryBridge& Bridge;
};
