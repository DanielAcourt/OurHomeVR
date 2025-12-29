#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidTypes.h"
#include "Engine/EngineTypes.h"
#include "BeeActor.generated.h"

class AHiveManager;
class USaveableEntityComponent;
class UQiComponent;
class UInteractionComponent;

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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USaveableEntityComponent* SaveableEntityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UQiComponent* QiComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UInteractionComponent* InteractionComponent;

public:
    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float InteractionDistance = 150.0f;
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

    UPROPERTY(EditAnywhere, Category = "Wisp|Boids")
    float TargetWeight = 2.0f;

    UPROPERTY(VisibleInstanceOnly, Category = "Wisp|Boids")
    AHiveManager* Manager;

    UPROPERTY(VisibleInstanceOnly, Category = "Wisp|Boids")
    AActor* TargetActor;

    FGuid DiscoveredTargetID;

    UPROPERTY(VisibleInstanceOnly, Category = "Wisp|Boids")
    EBeeState BeeState = EBeeState::Idle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Payload")
    float MaxPayload;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Payload")
    float CurrentPayload;

    FVector GetCurrentVelocity() const { return CurrentVelocity; }

private:
    void FindNewTarget();
    FVector CalculateFlockingForces(const FVector& GroupVelocity, const FVector& GroupCenter);
    void UpdateMovement(const FVector& Steering, float DeltaTime);

    void HandleIdleState();
    void HandleScoutingState();
    void HandleMovingToTargetState();
    void HandleReturningToHiveState();
    void HandleInteractingState();
    void HandleRestingState();

    TArray<ABeeActor*> GetNearbySisters();
    void ApplyGeneticDNA();
    void SynchronizeSymmetryWithComponents();
    FVector CurrentVelocity;
    bool ValidatePhysicalSymmetry(FVector TargetLocation);
    void EnterSuccessionState();
    FTimerHandle StateTimer;
};
