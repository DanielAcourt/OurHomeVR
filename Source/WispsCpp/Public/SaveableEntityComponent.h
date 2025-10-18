#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GardenData.h" // Include our new data header
#include "SaveableEntityComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPSCPP_API USaveableEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USaveableEntityComponent();

protected:
	virtual void BeginPlay() override;

public:
	// A unique ID for this specific instance (this one plant, this one butterfly)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGuid InstanceID;

	// The Twitch User who owns this entity
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	//FGuid OwnerTwitchID;

	// A tag defining what this entity IS. e.g., "Garden.EntityType.Soil"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGameplayTag EntityTypeTag;

	// Current work in progress
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	EGrowthState CurrentState;

	// Defines how often the GardenManager should update this component's owner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	EUpdateFrequency UpdateGroup = EUpdateFrequency::Medium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Training")
	ETrainingFocus CurrentFocus = ETrainingFocus::TF_None;

	// The UI Widget to open when this entity is interacted with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	
};
