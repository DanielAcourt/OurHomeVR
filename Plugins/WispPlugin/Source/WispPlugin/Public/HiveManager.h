#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "BeeActor.h"
#include "BoidTypes.h"
#include "HiveManager.generated.h"

class USaveableEntityComponent;

UCLASS()
class WISPPLUGIN_API AHiveManager : public AInfo
{
	GENERATED_BODY()

public:
	AHiveManager();

    UFUNCTION(BlueprintCallable, Category = "Wisp|Hive")
    void SpawnBees(EBeeType BeeType, int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Wisp|Hive|Discovery")
    void RegisterProbableTarget(const FGuid& TargetID);

    UFUNCTION(BlueprintCallable, Category = "Wisp|Hive|Discovery")
    void PromoteToTangible(const FGuid& TargetID);

    UFUNCTION(BlueprintCallable, Category = "Wisp|Hive|Discovery")
    ETruthState GetTruthState(const FGuid& TargetID) const;

    bool RequestTarget(FGuid& OutTargetID);
    void PromoteTargetIfScoutReturns(const FGuid& TargetID);
    void DemoteToUnknown(const FGuid& TargetID);
    TArray<ABeeActor*> GetNearbyBees(const FVector& Location, float Radius);

    FGuid GetMyGuid() const { return HiveID; }

    float RequestHoney(float AmountRequested);
    void DepositPollen(float Amount);
    void UnregisterBee(ABeeActor* Bee);

protected:
    virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USaveableEntityComponent* SaveableEntityComponent;

    UPROPERTY(EditAnywhere, Category = "Wisp|Hive")
    TSubclassOf<ABeeActor> BeeActorTemplate;

	UPROPERTY(VisibleAnywhere, Category = "Wisp|Hive")
	FGuid HiveID;

	UPROPERTY(VisibleAnywhere, Category = "Wisp|Hive")
	int32 GenerationCounter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float HoneyReserves = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float PollenStorage = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float JellyEssence = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float MaxStorageCapacity = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float PollenToHoneyConversionRate = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float HoneyToJellyConversionRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Hive|Metabolism")
    float JellySynthesisThreshold = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	TArray<ABeeActor*> ManagedSwarm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wisp|Boids")
	float SwarmBounds = 1000.0f;

    UPROPERTY(VisibleAnywhere, Category = "Wisp|Hive|Discovery")
    TMap<FGuid, ETruthState> DiscoveryRegistry;

    UPROPERTY(EditAnywhere, Category = "Wisp|Hive|Discovery")
    FGameplayTagContainer ThreatTags;

    UPROPERTY(EditAnywhere, Category = "Wisp|Hive|Discovery")
    FGameplayTag FlowerTag;

private:
    FTimerHandle RegistryAuditTimer;
    FTimerHandle TargetSearchTimer;
    TQueue<FGuid> AvailableTargets;
    TMap<EBeeType, float> HoneyCostMap;
    TMap<FIntVector, TArray<ABeeActor*>> SpatialGrid;

    UPROPERTY(EditAnywhere, Category = "Wisp|Hive")
    float GridCellSize = 1000.0f;

    void UpdateMetabolism(float DeltaTime);
	void RegistryAudit();
    void SearchForNewTargets();
    void UpdateSpatialGrid();
	FVector CalculateAverageVelocity();
	FVector CalculateSwarmCenter();
};
