#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTASimulator.generated.h"

// Struct to hold the results of a single simulation run
USTRUCT(BlueprintType)
struct FPSTASSimulationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float GPOSStress;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_detect;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_prop;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_entry;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_SSA;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_TAV;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float WCET_actuate;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    float TotalLatency;

    UPROPERTY(BlueprintReadOnly, Category = "PSTAS Simulation")
    bool bDeadlineMet;
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
    void RunSimulation();
    void LogEvent(const FString& EventMessage);
    void LogSimulationSummary(const TArray<FPSTASSimulationResult>& Results, bool bIsolationLemmaVerified);

    // Simulation Input Parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|Inputs")
    int32 N = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|Inputs")
    float Ts = 0.001f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|Inputs")
    float T_crit = 0.010f;

    // WCET Hardware/Software Constants
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_gate = 0.0001f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_prop_max = 0.00005f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_entry_max = 0.0002f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_triage_max = 0.0005f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_validate_max = 0.0003f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSTAS Simulation|WCET Constants")
    float t_actuate_max = 0.00015f;

    // This is now used as an iterator in the Isolation Lemma test
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSTAS Simulation|State")
    float GPOSStressParameter = 0.0f;
};
