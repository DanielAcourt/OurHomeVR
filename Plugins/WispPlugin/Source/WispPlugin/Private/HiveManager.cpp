// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "HiveManager.h"
#include "BeeActor.h"
#include "Engine/World.h"
#include "BoidTypes.h"

AHiveManager::AHiveManager() {
    PrimaryActorTick.bCanEverTick = true;
    GenerationCounter = 0;
}

void AHiveManager::BeginPlay()
{
    Super::BeginPlay();
    HiveID = FGuid::NewGuid();
}

void AHiveManager::SpawnBees(int32 Count)
{
    if (!BeeActorTemplate) return;

    UWorld* World = GetWorld();
    if (!World) return;

    GenerationCounter++;

    for (int32 i = 0; i < Count; ++i)
    {
        FVector SpawnLocation = GetActorLocation() + FMath::VRand() * SwarmBounds / 2.0f;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ABeeActor* NewBee = World->SpawnActor<ABeeActor>(BeeActorTemplate, SpawnLocation, SpawnRotation);
        if (NewBee)
        {
            NewBee->InitializeIdentity(HiveID, GenerationCounter, EBeeType::Worker);
            NewBee->Manager = this;
            ManagedSwarm.Add(NewBee);
        }
    }
}

void AHiveManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

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
