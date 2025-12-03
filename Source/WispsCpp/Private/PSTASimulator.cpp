#include "PSTASimulator.h"
#include "pstast_core_config.h" // The single source of truth for PNDF logic.
#include "Json.hh"
#include "JsonUtilities.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

APSTASimulator::APSTASimulator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APSTASimulator::BeginPlay()
{
	Super::BeginPlay();

    if (TasksToRun.Num() == 0)
    {
        const TArray<double> NominalBudget = {0.005, 0.001, 0.002, 0.015, 0.007, 0.006};
        TasksToRun.Add(FSimulatedTask("Safety_Brake_System", NominalBudget, 0.98, 1.0));
        TasksToRun.Add(FSimulatedTask("Propulsion_Override", NominalBudget, 0.99, 1.2));
        TasksToRun.Add(FSimulatedTask("State_Machine_Integrity", NominalBudget, 0.94, 1.0));
    }

	RunSimulationExecutive();
    LogSimulationSummary();
}

void APSTASimulator::RunSimulationExecutive()
{
    for (FSimulatedTask& Task : TasksToRun)
    {
        PSTAS_Core::run_pndf_check(Task);
    }
}

void APSTASimulator::LogSimulationSummary()
{
    // --- JSON REPORT CONSTRUCTION ---
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> LogEntry = MakeShareable(new FJsonObject);
    LogEntry->SetStringField(TEXT("timestamp"), FDateTime::UtcNow().ToIso8601());
    LogEntry->SetStringField(TEXT("AI_name"), TEXT("Jules"));
    LogEntry->SetStringField(TEXT("tier"), 3);
    LogEntry->SetStringField(TEXT("task_description"), TEXT("PSTAS Final Compliance Simulation Report"));

    TArray<TSharedPtr<FJsonValue>> TaskResultsArray;
    for (const FSimulatedTask& Task : TasksToRun)
    {
        TSharedPtr<FJsonObject> TaskObject = MakeShareable(new FJsonObject);
        TaskObject->SetStringField(TEXT("TaskName"), Task.TaskName);
        TaskObject->SetStringField(TEXT("PNDF_Status"), Task.bViolationDetected ? TEXT("VIOLATION") : TEXT("COMPLIANT"));

        // Add Stage-by-Stage Breakdown
        TSharedPtr<FJsonObject> WcetBreakdown = MakeShareable(new FJsonObject);
        WcetBreakdown->SetNumberField(TEXT("1_WCET_detect_s"), Task.NominalWCET_Budget[0]);
        WcetBreakdown->SetNumberField(TEXT("2_WCET_prop_s"),   Task.NominalWCET_Budget[1]);
        WcetBreakdown->SetNumberField(TEXT("3_WCET_entry_s"),  Task.NominalWCET_Budget[2]);
        WcetBreakdown->SetNumberField(TEXT("4_WCET_SSA_s"),    Task.NominalWCET_Budget[3]);
        WcetBreakdown->SetNumberField(TEXT("5_WCET_TAV_s"),    Task.NominalWCET_Budget[4]);
        WcetBreakdown->SetNumberField(TEXT("6_WCET_actuate_s"),Task.NominalWCET_Budget[5]);
        TaskObject->SetObjectField(TEXT("Nominal_WCET_Breakdown"), WcetBreakdown);

        TaskObject->SetNumberField(TEXT("Nominal_L_PSTAS_s"), Task.Nominal_L_PSTAS);
        TaskObject->SetNumberField(TEXT("Measured_WCET_s"), Task.Measured_WCET_Sim);
        TaskObject->SetNumberField(TEXT("Measured_TAV"), Task.SimulatedTAV);

        TaskResultsArray.Add(MakeShareable(new FJsonValueObject(TaskObject)));
    }

    RootObject->SetObjectField(TEXT("log_entry"), LogEntry);
    RootObject->SetArrayField(TEXT("simulation_results"), TaskResultsArray);

    // --- SERIALIZATION AND PERSISTENCE ---
    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&JsonString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

    const FString FinalReport = FString::Printf(TEXT("#RoGGGeeeRrrrr##\nPSTAS_Output_v2_Jules: FINAL REPORT\n%s"), *JsonString);

    // Log to console for immediate feedback
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalReport);

    // Persist to file in Saved/Logs directory
    const FString FilePath = FPaths::ProjectSavedDir() / TEXT("Logs") / TEXT("PSTAS_Simulation_Log.json");
    if (FFileHelper::SaveStringToFile(FinalReport, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("PSTAS simulation log saved to: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save PSTAS simulation log to file!"));
    }
}
