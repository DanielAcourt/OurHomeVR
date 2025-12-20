#pragma once

#include "ISymmetryKernel.h"
#include <functional>
#include <string>
#include <any>
#include <memory>

// Forward-declaration of the concrete kernel implementation.
class FSymmetryKernel;

/** @struct FRequestTruthData */
struct FRequestTruthData
{
    uint64_t SignalHash;
    std::any SignalData;
};

/** @struct FEquilibriumData */
struct FEquilibriumData
{
    uint64_t SignalHash;
    std::string SynthesizedTag;
};

/**
 * @class FSymmetryBridge
 * @brief A pure C++ communication bridge to decouple the Symmetry Kernel from its environment.
 */
class FSymmetryBridge
{
public:
    std::function<void(const FRequestTruthData&)> OnRequestTruth;
    std::function<void(const FEquilibriumData&)> OnEquilibriumReached;

    FSymmetryBridge();
    ~FSymmetryBridge();

    void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation);
    void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag);
    void SetParentalAuthorityWeight(float InWeight);

    /**
     * @brief Provides access to the concrete kernel instance for persistence operations.
     * @return A pointer to the concrete FSymmetryKernel implementation.
     */
    FSymmetryKernel* GetKernel();

private:
    friend class FSymmetryKernel;
    std::unique_ptr<ISymmetryKernel> KernelInstance;
};
