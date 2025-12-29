// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "BeeActor.h"
#include "HiveManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Misc/Guid.h"
#include "Hash/CityHash.h"
#include "SaveableEntityComponent.h"
#include "QiComponent.h"
#include "GameplayTagContainer.h"
#include "EngineUtils.h"
#include "ActorRegistry.h"

ABeeActor::ABeeActor() {
    PrimaryActorTick.bCanEverTick = false;
    CurrentVelocity = FMath::VRand();

    SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));
    QiComponent = CreateDefaultSubobject<UQiComponent>(TEXT("QiComponent"));
}

// ... (BeginPlay, EndPlay, InitializeIdentity, ApplyGeneticDNA, SynchronizeSymmetryWithComponents, GetNearbySisters, FindNewTarget, CalculateFlockingForces, UpdateMovement are the same)

void ABeeActor::UpdateBee(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter) {
    if (GeneticProfile.BeeType == EBeeType::Nursery) return;

    if (!TargetActor)
    {
        FindNewTarget();
    }
    else
    {
        USaveableEntityComponent* TargetSaveComponent = TargetActor->FindComponentByClass<USaveableEntityComponent>();
        if (TargetSaveComponent && Manager->GetTruthState(TargetSaveComponent->InstanceID) == ETruthState::Dissonance && GeneticProfile.BeeType != EBeeType::Drone)
        {
            TargetActor = Manager; // Flee
        }

        if (FVector::DistSquared(GetActorLocation(), TargetActor->GetActorLocation()) < 10000.0f) // 100*100
        {
            if (GeneticProfile.BeeType == EBeeType::Scout && TargetActor != Manager)
            {
                DiscoveredTargetID = TargetSaveComponent->InstanceID;
                TargetActor = Manager;
            }
            else if (TargetActor == Manager && DiscoveredTargetID.IsValid())
            {
                Manager->PromoteToTangible(DiscoveredTargetID);
                DiscoveredTargetID.Invalidate();
                TargetActor = nullptr;
            }
        }
    }

    SynchronizeSymmetryWithComponents();

    if (!Manager) {
        EnterSuccessionState(DeltaTime);
        return;
    }

    FVector Steering = CalculateFlockingForces(GroupVelocity, GroupCenter).GetClampedToMaxSize(MaxSteeringForce);
    UpdateMovement(Steering, DeltaTime);
}

// ... (ValidatePhysicalSymmetry, EnterSuccessionState are the same)
