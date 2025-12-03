// =============================================================================
// pstast_core_config.h
// Foundational Execution Safety Module for the PSTAS Framework
//
// DANIEL ACOURT REVIEW NOTES:
// This header establishes the core, non-negotiable safety parameters and
// validation logic for the Provably Safe, Trust-Aware System (PSTAS). It
// serves as the single source of truth for all safety-critical checks.
// =============================================================================

#pragma once

#include "CoreMinimal.h"
#include "PSTASimulator.h" // Required for the FSimulatedTask structure.

// Namespace for all PSTAS core functionalities to prevent global scope pollution.
namespace PSTAS_Core
{
    // =========================================================================
    // SECTION 1: DETERMINISTIC SAFETY CONSTANTS
    // =========================================================================

    /**
     * @brief The mandatory minimum "Percentage of Truth" required for an
     * operation to be considered safe.
     */
    constexpr double TAV_THRESHOLD = 0.950000;

    // =========================================================================
    // SECTION 2: PNDF VALIDATION FUNCTION (SINGLE SOURCE OF TRUTH)
    // =========================================================================

    /**
     * @brief Implements the mandatory PSTAS safety predicate (PNDF Standard Check).
     * This is the definitive, single source of truth for all compliance checks.
     * @param TaskResult The FSimulatedTask structure containing nominal and measured results.
     * @return bool - TRUE if a violation is detected, FALSE if compliant.
     */
    inline bool run_pndf_check(FSimulatedTask& TaskResult)
    {
        // 1. Calculate the measured WCET based on the simulation multiplier.
        TaskResult.Measured_WCET_Sim = TaskResult.Nominal_L_PSTAS * TaskResult.SimulatedWCET_Multiplier;

        // 2. Check for WCET Overrun (Timing Failure).
        const bool bWcetFailure = (TaskResult.Measured_WCET_Sim > TaskResult.Nominal_L_PSTAS);

        // 3. Check for TAV Underrun (Truth Failure).
        const bool bTavFailure = (TaskResult.SimulatedTAV < TAV_THRESHOLD);

        // 4. Determine final compliance. A violation occurs if either check fails.
        TaskResult.bViolationDetected = bWcetFailure || bTavFailure;

        return TaskResult.bViolationDetected;
    }

} // namespace PSTAS_Core
