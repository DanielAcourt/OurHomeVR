// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "PlantDataComponent.generated.h"

/**
 * @class UPlantDataComponent
 * @brief An actor component that holds data for a plant.
 *
 * This component stores an FPlantData struct, which contains all the relevant
 * information about a plant, such as its growth state, attributes, and Qi data.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UPlantDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UPlantDataComponent();

	/**
	 * @brief The data for the plant.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FPlantData PlantData;

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
