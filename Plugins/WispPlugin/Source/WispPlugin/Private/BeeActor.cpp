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

void ABeeActor::InitializeIdentity(const FGuid& HiveID, int32 GenerationIndex, EBeeType BeeType)
{
    GeneticProfile.HomeHiveID = HiveID;

    FString HashedString = FString::Printf(TEXT("%s-%d-%d"), *HiveID.ToString(), GenerationIndex, static_cast<int32>(BeeType));
    uint64 Hash1 = CityHash64(TCHAR_TO_ANSI(*HashedString), HashedString.Len());
    uint64 Hash2 = CityHash64(TCHAR_TO_ANSI(*HashedString), HashedString.Len()) + 1;
    uint32* Hash1AsInts = reinterpret_cast<uint32*>(&Hash1);
    uint32* Hash2AsInts = reinterpret_cast<uint32*>(&Hash2);
    GeneticProfile.LineageID = FGuid(Hash1AsInts[0], Hash1AsInts[1], Hash2AsInts[0], Hash2AsInts[1]);
    GeneticProfile.GeneticSeed.Initialize(GeneticProfile.LineageID.A);

    if (SaveableEntityComponent)
    {
        SaveableEntityComponent->InstanceID = GeneticProfile.LineageID;
    }
}

ABeeActor::ABeeActor() {
    PrimaryActorTick.bCanEverTick = false;
    EpistemicState = 0b11;
    CurrentVelocity = FMath::VRand();

    SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));
    QiComponent = CreateDefaultSubobject<UQiComponent>(TEXT("QiComponent"));
}

void ABeeActor::BeginPlay()
{
    Super::BeginPlay();
    if (GeneticProfile.GeneticSeed.IsInitialized())
    {
        MaxSpeed = GeneticProfile.GeneticSeed.FRandRange(400.0f, 600.0f);
        MaxSteeringForce = GeneticProfile.GeneticSeed.FRandRange(80.0f, 120.0f);
        QiCapacity = GeneticProfile.GeneticSeed.FRandRange(80.0f, 150.0f);

        QiComponent->QiData.MaxQi = QiCapacity;
        QiComponent->QiData.CurrentQi = QiCapacity;
    }

    if (SaveableEntityComponent)
    {
        SaveableEntityComponent->EntityTypeTag = FGameplayTag::RequestGameplayTag(FName("Animal.Insect.Bee"));
    }
}

void ABeeActor::SynchronizeSymmetryWithComponents()
{
    if (QiComponent)
    {
        float QiPercentage = FMath::Max(0.0f, QiComponent->QiData.CurrentQi) / QiComponent->QiData.MaxQi;
        float CurrentMaxSpeed = MaxSpeed * FMath::Clamp(QiPercentage, 0.1f, 1.0f);
        CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(CurrentMaxSpeed);
    }
}

TArray<ABeeActor*> ABeeActor::GetNearbySisters()
{
    TArray<ABeeActor*> NearbySisters;
    if (!Manager) return NearbySisters;

    for (ABeeActor* OtherBee : Manager->ManagedSwarm)
    {
        if (OtherBee != this && OtherBee->GeneticProfile.HomeHiveID == this->GeneticProfile.HomeHiveID)
        {
            if (FVector::Dist(GetActorLocation(), OtherBee->GetActorLocation()) < PerceptionRadius)
            {
                NearbySisters.Add(OtherBee);
            }
        }
    }
    return NearbySisters;
}

void ABeeActor::UpdateBee(float DeltaTime, const FVector& GroupVelocity, const FVector& GroupCenter) {
    SynchronizeSymmetryWithComponents();

    if (EpistemicState < 0b11 || !Manager) {
        EnterSuccessionState(DeltaTime);
        return;
    }

    TArray<ABeeActor*> NearbySisters = GetNearbySisters();

    FVector SeparationVector = FVector::ZeroVector;
    if (NearbySisters.Num() > 0)
    {
        for (ABeeActor* Sister : NearbySisters)
        {
            float Distance = FVector::Dist(GetActorLocation(), Sister->GetActorLocation());
            if (Distance > 0 && Distance < PerceptionRadius / 2.0f)
            {
                SeparationVector -= (Sister->GetActorLocation() - GetActorLocation()).GetSafeNormal() / Distance;
            }
        }
        SeparationVector /= NearbySisters.Num();
    }

    FVector CohesionVector = (GroupCenter - GetActorLocation());
    FVector AlignmentVector = (GroupVelocity - CurrentVelocity);
    FVector AvoidanceVector = FVector::ZeroVector;
    if (!ValidatePhysicalSymmetry(GetActorLocation() + CurrentVelocity.GetSafeNormal() * AvoidanceDistance))
    {
        AvoidanceVector = FVector::CrossProduct(CurrentVelocity, FVector::UpVector);
    }

    FVector BoundsVector = FVector::ZeroVector;
    if (FVector::Dist(GetActorLocation(), Manager->GetActorLocation()) > Manager->SwarmBounds)
    {
        BoundsVector = (Manager->GetActorLocation() - GetActorLocation());
    }

    FVector Acceleration = FVector::ZeroVector;
    Acceleration += SeparationVector.GetSafeNormal() * SeparationWeight;
    Acceleration += CohesionVector.GetSafeNormal() * CohesionWeight;
    Acceleration += AlignmentVector.GetSafeNormal() * AlignmentWeight;
    Acceleration += AvoidanceVector.GetSafeNormal() * AvoidanceWeight;
    Acceleration += BoundsVector.GetSafeNormal() * 1.5f;

    FVector Steering = Acceleration.GetClampedToMaxSize(MaxSteeringForce);
    CurrentVelocity += Steering * DeltaTime;

    SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
    SetActorRotation(CurrentVelocity.Rotation());
}

bool ABeeActor::ValidatePhysicalSymmetry(FVector TargetLocation) {
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    return !GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), TargetLocation, ECC_Visibility, CollisionParams);
}

void ABeeActor::EnterSuccessionState(float DeltaTime)
{
    CurrentVelocity *= 0.95f;
	SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
    SetActorRotation(CurrentVelocity.Rotation());
}
