#pragma once

#include "ISymmetryKernel.h"
#include "SymmetryBridge.h"
#include <unordered_map>
#include <string>
#include <algorithm>

/**
 * @class FSymmetryKernel
 * @brief Concrete implementation of the ISymmetryKernel interface.
 * @note This header is private to the SymmetryKernel module.
 */
class FSymmetryKernel : public ISymmetryKernel
{
public:
    explicit FSymmetryKernel(FSymmetryBridge& InBridge);
    virtual ~FSymmetryKernel() override = default;

    //~ Begin ISymmetryKernel Interface
    virtual void ProcessSignal(uint64_t InSignalHash, std::any InSignalData, bool bSensorConfirmation) override;

    /**
     * @brief Overrides the base interface, ensuring the signature matches exactly.
     */
    virtual void ProvideTruth(uint64_t InSignalHash, const std::string& InSynthesizedTag) override;

    virtual void SetParentalAuthorityWeight(float InWeight) override;
    //~ End ISymmetryKernel Interface

private:
    std::unordered_map<uint64_t, ESymmetryState> StateMatrix;
    std::unordered_map<uint64_t, std::string> ArchivalMemory;
    std::unordered_map<uint64_t, float> SignalTrust;
    float ParentalAuthorityWeight = 1.0f;
    const float TRUST_THRESHOLD = 0.2f;
    FSymmetryBridge& Bridge;
};
