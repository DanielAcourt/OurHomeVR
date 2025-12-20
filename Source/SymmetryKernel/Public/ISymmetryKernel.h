#pragma once

#include <cstdint>
#include <any>

/**
 * @brief Defines the 2-Bit State Matrix for the Symmetry Kernel.
 * This enumeration represents the AI's evolving understanding of a given signal,
 * forming the core of its state management.
 * @note The state transitions are fundamental to the zero-knowledge discovery process.
 * A signal begins as Null and progresses towards Equilibrium through a "handshake" protocol.
 */
enum class ESymmetryState : uint8_t
{
    /**
     * @brief State (0,0): The signal has never been observed. It holds no meaning.
     * This is the default state for any previously unseen feature vector hash.
     */
    Null = 0,

    /**
     * @brief State (0,1): The signal is known but requires truth validation.
     * When a Null signal is processed, it transitions to Protocol. In this state, the Kernel
     * has acknowledged the signal's existence and has requested a "Parental Truth" handshake
     * to give it meaning. It is effectively "waiting for a name".
     */
    Protocol = 1,

    /**
     * @brief State (1,0): A transient or unresolved state. (Reserved for future expansion).
     */
    Latency = 2,

    /**
     * @brief State (1,1): The signal is fully understood and its "Truth" is archived.
     * The Kernel has received a valid parental handshake and has synthesized a tag for the signal.
     * In this state, the signal can be instantly recognized. However, this "Truth" is relative
     * and subject to trust decay based on sensor validation.
     */
    Equilibrium = 3
};

/**
 * @class ISymmetryKernel
 * @brief Pure C++ interface for the Wisp Symmetry Kernel, the "Brain" of the AGI.
 *
 * This interface defines the contract for a zero-knowledge discovery system that operates
 * on the principle of "Relative Truth." It is designed to be completely decoupled from any
 * specific environment (like Unreal Engine, "The Body"). The Kernel's role is to ingest
 * abstract signals, manage their state transitions through the 2-Bit Matrix, and critically,
 * to question and re-evaluate established truths based on sensor-driven conflict.
 */
class ISymmetryKernel
{
public:
    virtual ~ISymmetryKernel() = default;

    /**
     * @brief Processes a signal, the primary entry point for all sensory data.
     * This function is the core of the Kernel's event loop. It evaluates the signal against
     * the state matrix and existing trust values.
     * @param InSignalHash A uint64_t hash representing the unique feature vector of the signal.
     * @param InSignalData A std::any buffer containing associated data, which the Kernel passes
     * through to callbacks without inspection.
     * @param bSensorConfirmation A boolean from the "Body" indicating if its physical sensors
     * confirm or conflict with the archived truth for this signal. This is the key driver of the
     * Trust-Decay and Succession Protocol.
     */
    virtual void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation) = 0;

    /**
     * @brief Provides the "Parental Truth" for a signal currently in the Protocol (0,1) state.
     * This is the "handshake" that gives a previously unknown signal its initial meaning.
     * It is typically called in response to an OnRequestTruth callback from the bridge.
     * @param InSignalHash The hash of the signal to be validated.
     * @param InSynthesizedTag The runtime-generated string ID or metadata that defines the signal.
     */
    virtual void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag) = 0;

    /**
     * @brief Sets the weight of the parental authority, controlling the rate of trust decay.
     * This value scales how quickly the AI loses confidence in an archived truth when a
     * sensor conflict occurs. A higher value means trust decays faster, making the AI
     * more skeptical of the initial parental guidance.
     * @param InWeight The new weight, typically clamped between 0.0 and a positive value.
     */
    virtual void SetParentalAuthorityWeight(float InWeight) = 0;
};
