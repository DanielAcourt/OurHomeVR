// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "HiveManager.h"
#include "BeeActor.h"
#include "Engine/World.h"
#include "BoidTypes.h"
#include "SaveableEntityComponent.h"
#include "TimerManager.h"
#include "ActorRegistry.h"
#include "DrawDebugHelpers.h"

AHiveManager::AHiveManager() {
    PrimaryActorTick.bCanEverTick = true;
    GenerationCounter = 0;

    SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));

    HoneyCostMap.Add(EBeeType::Scout, 1.0f);
    HoneyCostMap.Add(EBeeType::Gatherer, 3.0f);
    HoneyCostMap.Add(EBeeType::Nursery, 2.0f);
    HoneyCostMap.Add(EBeeType::Drone, 5.0f);
}

void AHiveManager::BeginPlay()
{
    Super::BeginPlay();
    HiveID = FGuid::NewGuid();
    if (SaveableEntityComponent)
    {
        SaveableEntityComponent->InstanceID = HiveID;
    }

    GetWorld()->GetTimerManager().SetTimer(RegistryAuditTimer, this, &AHiveManager::RegistryAudit, 10.0f, true);
    GetWorld()->GetTimerManager().SetTimer(TargetSearchTimer, this, &AHiveManager::SearchForNewTargets, 5.0f, true);
}

void AHiveManager::SearchForNewTargets()
{
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(SwarmBounds * 2.0f);
    GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_WorldStatic, Sphere);

    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* Actor = Overlap.GetActor();
        if (Actor)
        {
            USaveableEntityComponent* SaveComponent = Actor->FindComponentByClass<USaveableEntityComponent>();
            if (SaveComponent && SaveComponent->InstanceID.IsValid() && !DiscoveryRegistry.Contains(SaveComponent->InstanceID))
            {
                if (SaveComponent->EntityTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Plant.Flower"))))
                {
                    AvailableTargets.Enqueue(SaveComponent->InstanceID);
                }
                else if (ThreatTags.HasTag(SaveComponent->EntityTypeTag))
                {
                    DiscoveryRegistry.Add(SaveComponent->InstanceID, ETruthState::Dissonance);
                }
            }
        }
    }
}

bool AHiveManager::RequestTarget(FGuid& OutTargetID)
{
    return AvailableTargets.Dequeue(OutTargetID);
}

void AHiveManager::SpawnBees(EBeeType BeeType, int32 Count)
{
    if (!BeeActorTemplate) return;

    UWorld* World = GetWorld();
    if (!World) return;

    float TotalCost = HoneyCostMap.FindRef(BeeType) * Count;
    if (TotalCost > HoneyReserves)
    {
        return;
    }

    HoneyReserves -= TotalCost;
    GenerationCounter++;

    for (int32 i = 0; i < Count; ++i)
    {
        FVector SpawnLocation = GetActorLocation() + FMath::VRand() * SwarmBounds / 2.0f;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ABeeActor* NewBee = World->SpawnActor<ABeeActor>(BeeActorTemplate, SpawnLocation, SpawnRotation);
        if (NewBee)
        {
            NewBee->InitializeIdentity(HiveID, GenerationCounter, BeeType);
            NewBee->Manager = this;
            ManagedSwarm.Add(NewBee);
        }
    }
}

void AHiveManager::RegisterProbableTarget(const FGuid& TargetID)
{
    if (!DiscoveryRegistry.Contains(TargetID))
    {
        DiscoveryRegistry.Add(TargetID, ETruthState::Probable);
    }
}

void AHiveManager::PromoteToTangible(const FGuid& TargetID)
{
    if (DiscoveryRegistry.Contains(TargetID))
    {
        DiscoveryRegistry[TargetID] = ETruthState::Tangible;
    }
}

ETruthState AHiveManager::GetTruthState(const FGuid& TargetID) const
{
    const ETruthState* State = DiscoveryRegistry.Find(TargetID);
    return State ? *State : ETruthState::Unknown;
}

void AHiveManager::PromoteTargetIfScoutReturns(const FGuid& TargetID)
{
    PromoteToTangible(TargetID);
}

// TODO: Add check for a "Dormant" state on the actor to fully comply with the user's directive.
void AHiveManager::RegistryAudit()
{
    UActorRegistry* Registry = GetGameInstance()->GetSubsystem<UActorRegistry>();
    if (!Registry) return;

    for (auto It = DiscoveryRegistry.CreateIterator(); It; ++It)
    {
        if (It->Value == ETruthState::Tangible)
        {
            if (!Registry->FindActor(It->Key))
            {
                It.RemoveCurrent();
            }
        }
    }
}

void AHiveManager::UpdateMetabolism(float DeltaTime)
{
    int32 NurseryBeeCount = 0;
    for (const ABeeActor* Bee : ManagedSwarm)
    {
        if (Bee && Bee->GeneticProfile.BeeType == EBeeType::Nursery)
        {
            NurseryBeeCount++;
        }
    }

    if (PollenStorage > 0 && NurseryBeeCount > 0)
    {
        float ConversionAmount = FMath::Min(PollenStorage, PollenToHoneyConversionRate * NurseryBeeCount * DeltaTime);
        if (HoneyReserves + ConversionAmount <= MaxStorageCapacity)
        {
            PollenStorage -= ConversionAmount;
            HoneyReserves += ConversionAmount;
        }
    }

    if (HoneyReserves > JellySynthesisThreshold)
    {
        float ExcessHoney = HoneyReserves - JellySynthesisThreshold;
        float ConversionAmount = FMath::Min(ExcessHoney, HoneyToJellyConversionRate * DeltaTime);
        HoneyReserves -= ConversionAmount;
        JellyEssence += ConversionAmount;
    }
}

void AHiveManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    UpdateMetabolism(DeltaTime);

    if (ManagedSwarm.Num() == 0)
    {
		return;
	}

    FVector AvgVel = CalculateAverageVelocity();
    FVector Center = CalculateSwarmCenter();

    for (ABeeActor* Bee : ManagedSwarm) {
        if (Bee) {
            Bee->UpdateBee(DeltaTime, AvgVel, Center);
        }
    }
}

FVector AHiveManager::CalculateAverageVelocity()
{
	FVector AverageVelocity = FVector::ZeroVector;
	int32 SwarmSize = ManagedSwarm.Num();
	if (SwarmSize == 0) return AverageVelocity;

	for (const ABeeActor* Bee : ManagedSwarm)
	{
		if (Bee)
		{
			AverageVelocity += Bee->GetCurrentVelocity();
		}
	}

	AverageVelocity /= SwarmSize;
	return AverageVelocity;
}

FVector AHiveManager::CalculateSwarmCenter()
{
	FVector SwarmCenter = FVector::ZeroVector;
	int32 SwarmSize = ManagedSwarm.Num();
	if (SwarmSize == 0) return SwarmCenter;

	for (const ABeeActor* Bee : ManagedSwarm)
	{
		if (Bee)
		{
			SwarmCenter += Bee->GetActorLocation();
		}
	}

	SwarmCenter /= SwarmSize;
	return SwarmCenter;
}
