// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem/SovereignGameData.h"
#include "SovereignSaveGame.generated.h"

/**
 * @class USovereignSaveGame
 * @brief The "Suitcase" object that acts as the container for our save data.
 * This object is serialized to and from the disk.
 */
UCLASS()
class WISPCPP_API USovereignSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** The master list of every saved entity in the world. */
	UPROPERTY(VisibleAnywhere, Category = "Sovereign")
	TArray<FEntitySaveData> SavedActors;

	/** High-level metadata for the save file. */
	UPROPERTY(VisibleAnywhere, Category = "Sovereign")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Sovereign")
	FDateTime SaveTime;

	// Global world data can be added here in the future (e.g., TimeOfDay).
};
