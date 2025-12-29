// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "BoidsManager.h"
#include "BoidsActor.h"

ABoidsManager::ABoidsManager() {
    PrimaryActorTick.bCanEverTick = true;
}

void ABoidsManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (ManagedFlock.Num() == 0)
    {
		return;
	}

    FVector AvgVel = CalculateAverageVelocity();
    FVector Center = CalculateFlockCenter();

    for (ABoidsActor* Boid : ManagedFlock) {
        if (Boid) {
            // The Umbilical: Passing authority to the Sons
            Boid->UpdateBoid(DeltaTime, AvgVel, Center);
        }
    }
}

FVector ABoidsManager::CalculateAverageVelocity()
{
	FVector AverageVelocity = FVector::ZeroVector;
	int32 FlockSize = ManagedFlock.Num();
	if (FlockSize == 0) return AverageVelocity;

	for (const ABoidsActor* Boid : ManagedFlock)
	{
		if (Boid)
		{
			AverageVelocity += Boid->GetCurrentVelocity();
		}
	}

	AverageVelocity /= FlockSize;
	return AverageVelocity;
}

FVector ABoidsManager::CalculateFlockCenter()
{
	FVector FlockCenter = FVector::ZeroVector;
	int32 FlockSize = ManagedFlock.Num();
	if (FlockSize == 0) return FlockCenter;

	for (const ABoidsActor* Boid : ManagedFlock)
	{
		if (Boid)
		{
			FlockCenter += Boid->GetActorLocation();
		}
	}

	FlockCenter /= FlockSize;
	return FlockCenter;
}
