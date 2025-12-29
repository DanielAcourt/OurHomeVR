// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GardenData.h"
#include "SaveableEntityComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API USaveableEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USaveableEntityComponent();

	// A unique ID for this specific instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGuid InstanceID;

	// A tag defining what this entity IS. e.g., "Garden.EntityType.Soil"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGameplayTag EntityTypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	EUpdateFrequency UpdateGroup = EUpdateFrequency::Medium;

	// The UI Widget to open when this entity is interacted with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
