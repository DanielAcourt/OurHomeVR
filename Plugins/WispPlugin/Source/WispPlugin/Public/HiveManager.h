// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "BeeActor.h"
#include "HiveManager.generated.h"

UCLASS()
class WISPPLUGIN_API AHiveManager : public AInfo
{
	GENERATED_BODY()

public:
	AHiveManager();

    UFUNCTION(BlueprintCallable, Category = "Wisp|Hive")
    void SpawnBees(int32 Count);

    FGuid GetMyGuid() const { return HiveID; }

protected:
    virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Wisp|Hive")
    TSubclassOf<ABeeActor> BeeActorTemplate;

	UPROPERTY(VisibleAnywhere, Category = "Wisp|Hive")
	FGuid HiveID;

	UPROPERTY(VisibleAnywhere, Category = "Wisp|Hive")
	int32 GenerationCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	TArray<ABeeActor*> ManagedSwarm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	float SwarmBounds = 1000.0f;

private:
	FVector CalculateAverageVelocity();
	FVector CalculateSwarmCenter();
};
