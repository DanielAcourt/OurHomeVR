// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SovereignSaveManager.generated.h"

/**
 * @class USovereignSaveManager
 * @brief Handles the serialization and deserialization of the game state.
 * This subsystem orchestrates the saving and loading process, interacting with all objects
 * that implement the ISovereignSaveInterface to gather and distribute their state data.
 */
UCLASS()
class WISPCPP_API USovereignSaveManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Saves the current state of the world to a specified slot.
	 * @param SlotName The name of the save slot.
	 * @return True if the save was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sovereign Save System")
	bool SaveGame(const FString& SlotName);

	/**
	 * Loads the game state from a specified slot.
	 * @param SlotName The name of the save slot.
	 * @return True if the load was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sovereign Save System")
	bool LoadGame(const FString& SlotName);

private:
	/** Helper function to get the full path for a save file. */
	FString GetSaveFilePath(const FString& SlotName) const;
};
