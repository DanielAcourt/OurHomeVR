#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTASimulator.generated.h"

// Forward declaration of the PNDF namespace to avoid circular dependencies if needed,
// but we rely on explicit include in the CPP for the function call.

/**
 * @struct FSimulatedTask
 * @brief Defines the configuration, simulation parameters, and results for a single PSTAS task.
 * This structure uses the mandatory 6-term WCET budget.
 */
USTRUCT(BlueprintType)
struct FSimulatedTask
{
    GENERATED_BODY()

    // --- CONFIGURATION ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation")
    FString TaskName;

    // The nominal WCET budget for each of the 6 stages of the PSTAS pipeline.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation")
    TArray<double> NominalWCET_Budget;

    // --- SIMULATION OVERRIDES (for testing failure cases) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation")
    double SimulatedWCET_Multiplier = 1.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation")
    double SimulatedTAV = 0.98;

    // --- RESULTS ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double Nominal_L_PSTAS = 0.0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double Measured_WCET_Sim = 0.0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    bool bViolationDetected = false;

    FSimulatedTask() {}

    FSimulatedTask(const FString& InName, const TArray<double>& InBudget, double InTAV = 0.98, double InWcetMultiplier = 1.0)
        : TaskName(InName),
          NominalWCET_Budget(InBudget),
          SimulatedWCET_Multiplier(InWcetMultiplier),
          SimulatedTAV(InTAV)
    {
        for (double StageWCET : NominalWCET_Budget)
        {
            Nominal_L_PSTAS += StageWCET;
        }
    }
};


UCLASS()
class WISCPSCPP_API APSTASimulator : public AActor
{
    GENERATED_BODY()

public:
    APSTASimulator();

    /**
     * @brief Executes the EOA Fallback logic for a given task.
     * This function is the entry point for the P0 safety floor check.
     * @param Task The task to run the PNDF check on.
     */
    void ExecuteEOAFallback(FSimulatedTask& Task);

protected:
    virtual void BeginPlay() override;

private:
    /**
     * @brief Defines the set of PSTAS tasks to run, configured in the editor.
     * This array uses the 6-term WCET budget from the FSimulatedTask struct.
     */
    UPROPERTY(EditAnywhere, Category = "PSTAS Executive")
    TArray<FSimulatedTask> TasksToRun;

    void RunSimulationExecutive();
    void LogSimulationSummary(); // Updated to remove parameter, uses TasksToRun array.
};
