// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GardenGridManager.generated.h"

class UGridDataComponent;

/**
 * @class AGardenGridManager
 * @brief Manages the garden grid system in the game.
 *
 * This actor is responsible for managing the data and logic of the garden grid.
 */
UCLASS()
class WISPPLUGIN_API AGardenGridManager : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	AGardenGridManager();

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief The component that holds the grid data.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UGridDataComponent> GridDataComponent;
};
