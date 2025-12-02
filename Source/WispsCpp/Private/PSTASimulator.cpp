#include "PSTASimulator.h"

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
    // Awaiting PNDF formal model from Gemini for implementation.
    LogData(TEXT("PSTAS_Output_v2_Jules: Simulation pipeline initiated. Awaiting PNDF model."));
}

void APSTASimulator::LogData(const FString& Message)
{
    FString FormattedMessage = FString::Printf(TEXT("#RoGGGeeeRrrrr##%s"), *Message);
    // Placeholder for logging to Dan's long-term storage.
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedMessage);
}
