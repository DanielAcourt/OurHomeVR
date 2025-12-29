// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidsActor.generated.h"

class ABoidsManager;

UCLASS()
class WISPPLUGIN_API ABoidsActor : public AActor
{
    GENERATED_BODY()

public:
    ABoidsActor();

    // The "Symmetry" update: Called by Manager, but validated by Kernel
    void UpdateBoid(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter);

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float CohesionWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float SeparationWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AlignmentWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float PerceptionRadius = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float MaxSpeed = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AvoidanceWeight = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AvoidanceDistance = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    ABoidsManager* Manager;

    FVector GetCurrentVelocity() const { return CurrentVelocity; }

private:
    // 2-Bit State: 11 = Normal, 10 = Guessing, 01 = Noisy, 00 = Failed
    uint8 EpistemicState;

    FVector CurrentVelocity;

    // The "Internal Truth" Check: Can I move here safely?
    bool ValidatePhysicalSymmetry(FVector TargetLocation);

    // The Succession Event: What to do if the Manager stops ticking
    void EnterSuccessionState(float DeltaTime);
};
