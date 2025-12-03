// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GardenData.h"
#include "SaveableEntityComponent.generated.h"

/**
 * @class USaveableEntityComponent
 * @brief An actor component that marks an actor as a saveable entity.
 *
 * This component provides a unique ID and other data necessary for saving and
 * loading an actor's state.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API USaveableEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	USaveableEntityComponent();

	/**
	 * @brief A unique ID for this specific instance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGuid InstanceID;

	/**
	 * @brief A tag defining what this entity IS. e.g., "Garden.EntityType.Soil".
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	FGameplayTag EntityTypeTag;

	/**
	 * @brief The update frequency for this entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	EUpdateFrequency UpdateGroup = EUpdateFrequency::Medium;

	/**
	 * @brief The UI Widget to open when this entity is interacted with.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveableEntity")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
