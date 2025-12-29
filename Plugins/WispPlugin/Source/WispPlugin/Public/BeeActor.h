// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidTypes.h"
#include "BeeActor.generated.h"

class AHiveManager;
class USaveableEntityComponent;
class UQiComponent;

UCLASS()
class WISPPLUGIN_API ABeeActor : public AActor
{
    GENERATED_BODY()

public:
    ABeeActor();

    void UpdateBee(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter);
    void InitializeIdentity(const FGuid& HiveID, int32 GenerationIndex, EBeeType BeeType);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Genetics")
    FBeeGeneticProfile GeneticProfile;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USaveableEntityComponent* SaveableEntityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UQiComponent* QiComponent;

public:
    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float CohesionWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float SeparationWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AlignmentWeight = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float PerceptionRadius = 1000.0f;

    UPROPERTY(VisibleAnywhere, Category = "Wisp|Boids")
    float MaxSpeed = 500.0f;

    UPROPERTY(VisibleAnywhere, Category = "Wisp|Boids")
    float MaxSteeringForce = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Wisp|Boids")
    float QiCapacity = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AvoidanceWeight = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float AvoidanceDistance = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float BoundsWeight = 1.5f;

    UPROPERTY(VisibleInstanceOnly, Category = "Wisp|Boids")
    AHiveManager* Manager;

    FVector GetCurrentVelocity() const { return CurrentVelocity; }

private:
    void ApplyGeneticDNA();
    void SynchronizeSymmetryWithComponents();
    uint8 EpistemicState;
    FVector CurrentVelocity;
    bool ValidatePhysicalSymmetry(FVector TargetLocation);
    void EnterSuccessionState(float DeltaTime);
};
