#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTASimulator.generated.h"

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
    void LogData(const FString& Data);

    // Placeholder for simulation parameters
    float GPOSStressParameter;
    float CriticalDeadline;
};
