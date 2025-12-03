#include "PSTASimulator.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "pstast_core_config.h" // Integrate the mandatory PNDF validation header.

APSTASimulator::APSTASimulator()
{
    PrimaryActorTick.bCanEverTick = false; // Enforce deterministic execution.
}

void APSTASimulator::BeginPlay()
{
    Super::BeginPlay();
    RunSimulationExecutive();
}

void APSTASimulator::RunSimulationExecutive()
{
    // DANIEL ACOURT REVIEW NOTE:
    // The simulation utilizes the 6-term WCET formula from the PSTAS-CAT Formal Model.
    // This is the mandatory, certifiable model and supersedes any prior, simplified
    // 5-term models. The 6 stages are: detect, prop, entry, SSA, TAV, and actuate.
    // ---

    // --- TASK DEFINITION ---
    // Define the list of tasks to be simulated, including two deliberate failure cases
    // to verify the PNDF check is functional.
    TArray<FSimulatedTask> TasksToExecute = {
        // --- NOMINAL (PASSING) CASES ---
        FSimulatedTask(TEXT("MainLoop_A"),       {0.001, 0.002, 0.003, 0.004, 0.005, 0.006}, 0.98),
        FSimulatedTask(TEXT("MotorControl_B"),   {0.001, 0.002, 0.002, 0.003, 0.003, 0.004}, 0.95),
        FSimulatedTask(TEXT("Trajectory_D"),     {0.001, 0.001, 0.002, 0.002, 0.003, 0.003}, 0.99),

        // --- FAILURE (VIOLATION) CASES ---
        // 1. WCET Overrun: Multiplier is set > 1.0
        FSimulatedTask(TEXT("SensorFusion_C"),   {0.005, 0.005, 0.005, 0.005, 0.005, 0.010}, 0.97, 1.50),
        // 2. TAV Underrun: TAV is set < TAV_THRESHOLD (0.95)
        FSimulatedTask(TEXT("SafetyBrake_E"),    {0.001, 0.001, 0.001, 0.001, 0.001, 0.001}, 0.850),
        // 3. Compound Failure (Both WCET Overrun and TAV Underrun)
        FSimulatedTask(TEXT("CriticalStop_F"),   {0.002, 0.001, 0.001, 0.001, 0.001, 0.002}, 0.750, 1.25)
    };

    // --- EXECUTION LOOP ---
    // Iterate through each defined task and run the simulation and PNDF check.
    for (FSimulatedTask& Task : TasksToExecute)
    {
        // 1. Simulate the measured WCET by applying the multiplier to the nominal total.
        Task.Measured_WCET_Sim = Task.Nominal_L_PSTAS * Task.SimulatedWCET_Multiplier;

        // 2. Perform the mandatory PNDF check using the function from the core config header.
        Task.bViolationDetected = PSTAS_Core::run_pndf_check(
            Task.Measured_WCET_Sim,
            Task.Nominal_L_PSTAS,
            Task.SimulatedTAV,
            PSTAS_Core::TAV_THRESHOLD
        );
    }

    // --- LOGGING ---
    // Pass the array of executed tasks to the logging function to generate the summary report.
    LogSimulationSummary(TasksToExecute);
}

void APSTASimulator::LogSimulationSummary(const TArray<FSimulatedTask>& ExecutedTasks)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> LogEntry = MakeShareable(new FJsonObject);
    LogEntry->SetStringField(TEXT("timestamp"), FDateTime::UtcNow().ToIso8601());
    LogEntry->SetStringField(TEXT("AI_name"), TEXT("Jules"));
    LogEntry->SetStringField(TEXT("tier"), 3);
    LogEntry->SetStringField(TEXT("task_description"), TEXT("PSTAS Multi-Task Simulation Executive Report"));
    LogEntry->SetStringField(TEXT("validation_status"), TEXT("Protocol Compliant"));

    TSharedPtr<FJsonObject> SimulationResults = MakeShareable(new FJsonObject);
    TArray<TSharedPtr<FJsonValue>> TaskResultsArray;

    for (const FSimulatedTask& Task : ExecutedTasks)
    {
        TSharedPtr<FJsonObject> TaskObject = MakeShareable(new FJsonObject);
        TaskObject->SetStringField(TEXT("TaskName"), Task.TaskName);
        TaskObject->SetNumberField(TEXT("Nominal_L_PSTAS_ms"), Task.Nominal_L_PSTAS * 1000); // Convert to ms for readability

        TSharedPtr<FJsonObject> WcetBreakdown = MakeShareable(new FJsonObject);
        WcetBreakdown->SetNumberField(TEXT("1_WCET_detect_ms"), Task.NominalWCET_Budget[0] * 1000);
        WcetBreakdown->SetNumberField(TEXT("2_WCET_prop_ms"),   Task.NominalWCET_Budget[1] * 1000);
        WcetBreakdown->SetNumberField(TEXT("3_WCET_entry_ms"),  Task.NominalWCET_Budget[2] * 1000);
        WcetBreakdown->SetNumberField(TEXT("4_WCET_SSA_ms"),    Task.NominalWCET_Budget[3] * 1000);
        WcetBreakdown->SetNumberField(TEXT("5_WCET_TAV_ms"),    Task.NominalWCET_Budget[4] * 1000);
        WcetBreakdown->SetNumberField(TEXT("6_WCET_actuate_ms"),Task.NominalWCET_Budget[5] * 1000);
        TaskObject->SetObjectField(TEXT("Nominal_WCET_Breakdown"), WcetBreakdown);

        TaskObject->SetNumberField(TEXT("Measured_WCET_ms"), Task.Measured_WCET_Sim * 1000); // Convert to ms
        TaskObject->SetNumberField(TEXT("Measured_TAV"), Task.SimulatedTAV);
        TaskObject->SetStringField(TEXT("PNDF_Status"), Task.bViolationDetected ? TEXT("VIOLATION") : TEXT("COMPLIANT"));

        TArray<FString> FailureTypes;
        if (Task.bViolationDetected)
        {
            if (Task.Measured_WCET_Sim > Task.Nominal_L_PSTAS)
            {
                FailureTypes.Add(TEXT("WCET Overrun"));
            }
            if (Task.SimulatedTAV < PSTAS_Core::TAV_THRESHOLD)
            {
                FailureTypes.Add(TEXT("TAV Underrun"));
            }
        }

        FString FailureTypeString = TEXT("N/A");
        if (FailureTypes.Num() > 0)
        {
            FailureTypeString = FString::Join(FailureTypes, TEXT(", "));
        }
        TaskObject->SetStringField(TEXT("FailureType"), FailureTypeString);

        TaskResultsArray.Add(MakeShareable(new FJsonValueObject(TaskObject)));
    }
    SimulationResults->SetArrayField("TaskSimulationResults", TaskResultsArray);

    JsonObject->SetObjectField(TEXT("log_entry"), LogEntry);
    JsonObject->SetObjectField(TEXT("simulation_results"), SimulationResults);

    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FString FormattedMessage = FString::Printf(TEXT("#RoGGGeeeRrrrr##\nPSTAS_Output_v2_Jules: EXECUTIVE SUMMARY:\n%s"), *JsonString);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedMessage);
}
