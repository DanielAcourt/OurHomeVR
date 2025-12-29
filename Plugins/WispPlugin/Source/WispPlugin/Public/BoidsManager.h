// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "BoidsActor.h"
#include "BoidsManager.generated.h"

UCLASS()
class WISPPLUGIN_API ABoidsManager : public AInfo
{
	GENERATED_BODY()

public:
	ABoidsManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	TArray<ABoidsActor*> ManagedFlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	float FlockBounds = 1000.0f;

protected:
	virtual void Tick(float DeltaTime) override;

private:
	// Calculate the "Shared Truth" for the flock
	FVector CalculateAverageVelocity();
	FVector CalculateFlockCenter();
};
