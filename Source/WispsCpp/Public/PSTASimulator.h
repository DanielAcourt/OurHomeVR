#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTASimulator.generated.h"

/**
 * @struct FSimulatedTask
 * @brief Holds the definition, simulation parameters, and results for a single PSTAS task.
 * This structure is used to configure and report on each simulation run within the executive.
 */
USTRUCT(BlueprintType)
struct FSimulatedTask
{
    GENERATED_BODY()

    // --- CONFIGURATION ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    FString TaskName;

    // The nominal WCET budget for each of the 6 stages of the PSTAS pipeline.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    TArray<double> NominalWCET_Budget;

    // --- SIMULATION OVERRIDES (for testing failure cases) ---
    // A multiplier applied to the nominal WCET to simulate a timing overrun. 1.0 = no overrun.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double SimulatedWCET_Multiplier = 1.0; // Default to 1.0 (no overrun). Overrides are set in task definitions.

    // The exact TAV score to report for this task, used to simulate TAV underruns.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double SimulatedTAV = 0.98;

    // --- RESULTS ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double Nominal_L_PSTAS = 0.0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    double Measured_WCET_Sim = 0.0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation")
    bool bViolationDetected = false;

    // Default constructor required by Unreal.
    FSimulatedTask() {}

    /**
     * @brief Convenience constructor to initialize a task.
     * @param InName The name of the task.
     * @param InBudget The 6-term WCET budget array.
     * @param InTAV The TAV score to simulate.
     * @param InWcetMultiplier A multiplier for simulating WCET overruns.
     */
    FSimulatedTask(const FString& InName, const TArray<double>& InBudget, double InTAV = 0.98, double InWcetMultiplier = 1.0)
        : TaskName(InName),
          NominalWCET_Budget(InBudget),
          SimulatedWCET_Multiplier(InWcetMultiplier),
          SimulatedTAV(InTAV)
    {
        // Pre-calculate the total nominal latency budget.
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

protected:
    virtual void BeginPlay() override;

private:
    void RunSimulationExecutive();
    void LogSimulationSummary(const TArray<FSimulatedTask>& ExecutedTasks);
};
