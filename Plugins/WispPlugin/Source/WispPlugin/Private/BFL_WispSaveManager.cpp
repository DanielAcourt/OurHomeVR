// Copyright Epic Games, Inc. All Rights Reserved.

#include "BFL_WispSaveManager.h"
#include "WispSaveGame.h"
#include "GardenGridManager.h"
#include "GridDataComponent.h"
#include "PlantDataComponent.h"
#include "SaveableEntityComponent.h"
#include "Kismet/GameplayStatics.h"

UWispSaveGame* UBFL_WispSaveManager::CreateSaveObjectFromWorld(const UObject* WorldContextObject)
{
	UWispSaveGame* SaveGame = NewObject<UWispSaveGame>();
	if (SaveGame == nullptr)
	{
		return nullptr;
	}

	// Save Soil Data from the Garden Grid Manager
	if (AGardenGridManager* GridManager = Cast<AGardenGridManager>(UGameplayStatics::GetActorOfClass(WorldContextObject, AGardenGridManager::StaticClass())))
	{
		if (GridManager->GridDataComponent)
		{
			SaveGame->SoilDataGrid = GridManager->GridDataComponent->SoilDataGrid;
		}
	}

	// Save Plant Data from all actors with a PlantDataComponent
	TArray<AActor*> PlantActors;
	UGameplayStatics::GetAllActorsWithComponent(WorldContextObject, UPlantDataComponent::StaticClass(), PlantActors);

	for (AActor* PlantActor : PlantActors)
	{
		if (UPlantDataComponent* PlantDataComp = PlantActor->FindComponentByClass<UPlantDataComponent>())
		{
			if (USaveableEntityComponent* SaveableComp = PlantActor->FindComponentByClass<USaveableEntityComponent>())
			{
				SaveGame->PlantDataMap.Add(SaveableComp->InstanceID, PlantDataComp->PlantData);
			}
		}
	}

	return SaveGame;
}

void UBFL_WispSaveManager::LoadWorldFromSaveObject(const UObject* WorldContextObject, UWispSaveGame* SaveGame)
{
	if (SaveGame == nullptr)
	{
		return;
	}

	// Load Soil Data to the Garden Grid Manager
	if (AGardenGridManager* GridManager = Cast<AGardenGridManager>(UGameplayStatics::GetActorOfClass(WorldContextObject, AGardenGridManager::StaticClass())))
	{
		if (GridManager->GridDataComponent)
		{
			GridManager->GridDataComponent->SoilDataGrid = SaveGame->SoilDataGrid;
		}
	}

	// Load Plant Data to all actors with a PlantDataComponent
	TArray<AActor*> PlantActors;
	UGameplayStatics::GetAllActorsWithComponent(WorldContextObject, UPlantDataComponent::StaticClass(), PlantActors);

	for (AActor* PlantActor : PlantActors)
	{
		if (USaveableEntityComponent* SaveableComp = PlantActor->FindComponentByClass<USaveableEntityComponent>())
		{
			if (SaveGame->PlantDataMap.Contains(SaveableComp->InstanceID))
			{
				if (UPlantDataComponent* PlantDataComp = PlantActor->FindComponentByClass<UPlantDataComponent>())
				{
					PlantDataComp->PlantData = SaveGame->PlantDataMap[SaveableComp->InstanceID];
				}
			}
		}
	}
}
