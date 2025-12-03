// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GardenData.h"
#include "WispSaveGame.generated.h"

/**
 * @class UWispSaveGame
 * @brief A USaveGame object for saving the state of the game.
 *
 * This class stores the data for the soil grid and all the plants in the world.
 */
UCLASS()
class WISPPLUGIN_API UWispSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/**
	 * @brief The saved data for the soil grid.
	 */
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TMap<FIntPoint, FSoilData> SoilDataGrid;

	/**
	 * @brief The saved data for all the plants in the world.
	 */
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TMap<FGuid, FPlantData> PlantDataMap;
};
