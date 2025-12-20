#pragma once

#include "ISymmetryKernel.h"
#include <functional>
#include <string>
#include <any>
#include <memory>

// Forward-declaration of the concrete kernel implementation.
// Hides the implementation details from the environment.
class FSymmetryKernel;

/**
 * @struct FRequestTruthData
 * @brief The data payload sent with the OnRequestTruth callback.
 * This struct packages the necessary information for the "Parental Authority" to identify
 * and provide a "Truth" handshake for a previously unknown signal.
 */
struct FRequestTruthData
{
    /** The unique hash of the signal that requires a truth definition. */
    uint64_t SignalHash;

    /** The original std::any payload, passed through for context. */
    std::any SignalData;
};

/**
 * @struct FEquilibriumData
 * @brief The data payload sent with the OnEquilibriumReached callback.
 * This struct informs the environment that a signal has reached a stable, understood state,
 * providing the newly synthesized tag associated with it.
 */
struct FEquilibriumData
{
    /** The unique hash of the signal that has reached equilibrium. */
    uint64_t SignalHash;

    /** The runtime-generated string tag now associated with this signal. */
    std::string SynthesizedTag;
};

/**
 * @class FSymmetryBridge
 * @brief A pure C++ communication bridge to decouple the Symmetry Kernel ("Brain") from its environment ("Body").
 *
 * This class is the single point of contact for any external application using the Kernel.
 * It achieves decoupling through two key mechanisms:
 * 1. PImpl Idiom: It owns a unique_ptr to the concrete ISymmetryKernel implementation, completely
 *    hiding the Kernel's internal logic and data structures from the public API.
 * 2. Callbacks: It uses std::function callbacks to allow the Kernel to send events back to the
 *    environment asynchronously, without the Kernel ever needing to know what the environment is
 *    (e.g., Unreal Engine, a console application, etc.).
 */
class FSymmetryBridge
{
public:
    /**
     * @brief Callback invoked when the Kernel encounters an unknown signal (Protocol state 0,1)
     * and requires "Parental Truth". The environment should subscribe to this event to provide
     * a synthesized tag via the ProvideTruth() method. This is the core of the "handshake" protocol.
     */
    std::function<void(const FRequestTruthData&)> OnRequestTruth;

    /**
     * @brief Callback invoked when a signal resolves to an Equilibrium state (1,1).
     * The environment should subscribe to this to be notified when a signal is fully understood
     * and its new tag is archived. This can be used to update the "Body's" own state.
     */
    std::function<void(const FEquilibriumData&)> OnEquilibriumReached;

    FSymmetryBridge();
    ~FSymmetryBridge();

    /**
     * @brief Forwards a signal from the environment to the kernel for processing.
     * @param InSignalHash A uint64_t hash representing the unique feature vector of the signal.
     * @param InSignalData A std::any buffer containing associated data, to be returned in callbacks.
     * @param bSensorConfirmation A boolean from the "Body" indicating if its sensors confirm the archived truth.
     */
    void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation);

    /**
     * @brief Forwards the "Truth" from the environment (Parental Authority) to the kernel.
     * @param InSignalHash The hash of the signal to be validated.
     * @param InSynthesizedTag A runtime-generated string ID or metadata that defines the signal.
     */
    void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag);

    /**
     * @brief Forwards the parental authority weight setting to the kernel.
     * @param InWeight The new weight, used to scale trust decay.
     */
    void SetParentalAuthorityWeight(float InWeight);

private:
    friend class FSymmetryKernel;

    /**
     * @brief A pointer to the concrete implementation of the kernel.
     * By using the ISymmetryKernel interface and a unique_ptr (PImpl Idiom), the bridge
     * completely hides the internal workings of the FSymmetryKernel from any client code.
     */
    std::unique_ptr<ISymmetryKernel> KernelInstance;
};
