// =============================================================================
// EOA_Executive_Interface.h
// Placeholder interface for the Executive Override Agent (EOA).
// This is a simplified, static class for the purpose of the EOA Fallback simulation.
// =============================================================================

#pragma once

#include "CoreMinimal.h"

namespace EOA_Executive_Interface
{
    // A static variable to simulate the state of the EOA.
    static bool bIsOverrideActive = true;

    /**
     * @brief Checks if the EOA override is currently active.
     * @return bool - True if the override is active.
     */
    inline bool IsOverrideActive()
    {
        return bIsOverrideActive;
    }

    /**
     * @brief Forcibly disables the EOA override.
     * This function is called by the P0 safety floor when a violation is detected.
     */
    inline void ForceOverrideShutdown()
    {
        if (bIsOverrideActive)
        {
            UE_LOG(LogTemp, Fatal, TEXT("[EOA FALLBACK] EOA override has been forcibly shut down by P0 safety floor due to a critical PNDF violation."));
            bIsOverrideActive = false;
        }
    }
}
