// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GardenData.h"
#include "WispSaveGame.generated.h"

UCLASS()
class WISPPLUGIN_API UWispSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TMap<FIntPoint, FSoilData> SoilDataGrid;

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TMap<FGuid, FPlantData> PlantDataMap;
};
