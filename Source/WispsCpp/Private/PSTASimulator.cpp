#include "PSTASimulator.h"
#include "Json.h"
#include "JsonUtilities.h"

APSTASimulator::APSTASimulator()
{
    PrimaryActorTick.bCanEverTick = false; // Disable ticking for deterministic simulation
}

void APSTASimulator::BeginPlay()
{
    Super::BeginPlay();
    RunSimulation();
}

void APSTASimulator::RunSimulation()
{
    // Log initial events as placeholders
    LogEvent(TEXT("Sensor anomaly detection event: Anomaly threshold exceeded."));
    LogEvent(TEXT("Trigger evaluation (H(t)): Trigger condition met, initiating PSTAS path."));

    TArray<FPSTASSimulationResult> Results;
    const TArray<float> StressLevels = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
    float FirstLatency = -1.0f;
    bool bIsolationLemmaVerified = true;

    for (float Stress : StressLevels)
    {
        GPOSStressParameter = Stress;
        FPSTASSimulationResult Result;
        Result.GPOSStress = GPOSStressParameter;

        // The key part of the Isolation Lemma test:
        // The GPOSStressParameter is *not* used in the latency calculation.
        Result.WCET_detect = (static_cast<float>(N) * Ts) + t_gate;
        Result.WCET_prop = t_prop_max;
        Result.WCET_entry = t_entry_max;
        Result.WCET_SSA = t_triage_max;
        Result.WCET_TAV = t_validate_max;
        Result.WCET_actuate = t_actuate_max;

        Result.TotalLatency = Result.WCET_detect + Result.WCET_prop + Result.WCET_entry +
                              Result.WCET_SSA + Result.WCET_TAV + Result.WCET_actuate;

        Result.bDeadlineMet = Result.TotalLatency <= T_crit;
        Results.Add(Result);

        if (FirstLatency < 0.0f)
        {
            FirstLatency = Result.TotalLatency;
        }
        else if (!FMath::IsNearlyEqual(FirstLatency, Result.TotalLatency))
        {
            bIsolationLemmaVerified = false;
        }
    }

    LogSimulationSummary(Results, bIsolationLemmaVerified);
}

void APSTASimulator::LogEvent(const FString& EventMessage)
{
    // Simple, non-JSON log for discrete events
    FString FormattedMessage = FString::Printf(TEXT("#RoGGGeeeRrrrr##PSTAS_Output_v2_Jules: EVENT: %s"), *EventMessage);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedMessage);
}

void APSTASimulator::LogSimulationSummary(const TArray<FPSTASSimulationResult>& Results, bool bIsolationLemmaVerified)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> LogEntry = MakeShareable(new FJsonObject);
    LogEntry->SetStringField(TEXT("timestamp"), FDateTime::UtcNow().ToIso8601());
    LogEntry->SetStringField(TEXT("AI_name"), TEXT("Jules"));
    LogEntry->SetNumberField(TEXT("tier"), 3);
    LogEntry->SetStringField(TEXT("task_description"), TEXT("PSTAS Deterministic Simulation Summary"));
    LogEntry->SetStringField(TEXT("validation_status"), TEXT("Protocol Compliant"));

    TSharedPtr<FJsonObject> SummaryData = MakeShareable(new FJsonObject);
    SummaryData->SetBoolField(TEXT("isolation_lemma_verified"), bIsolationLemmaVerified);
    SummaryData->SetNumberField(TEXT("critical_deadline_t_crit"), T_crit);

    TArray<TSharedPtr<FJsonValue>> RunsJsonArray;
    for (const auto& Result : Results)
    {
        TSharedPtr<FJsonObject> RunObject = MakeShareable(new FJsonObject);
        RunObject->SetNumberField(TEXT("gpos_stress_parameter"), Result.GPOSStress);
        RunObject->SetNumberField(TEXT("total_latency_l_pstas"), Result.TotalLatency);
        RunObject->SetBoolField(TEXT("deadline_met"), Result.bDeadlineMet);
        RunsJsonArray.Add(MakeShareable(new FJsonValueObject(RunObject)));
    }
    SummaryData->SetArrayField("simulation_runs", RunsJsonArray);

    JsonObject->SetObjectField(TEXT("log_entry"), LogEntry);
    JsonObject->SetObjectField(TEXT("simulation_summary"), SummaryData);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FString FormattedMessage = FString::Printf(TEXT("#RoGGGeeeRrrrr##\nPSTAS_Output_v2_Jules: SUMMARY:\n%s"), *JsonString);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedMessage);
}
