// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_UniversalSaveLoad.generated.h"

class USaveGame;

/**
 * @class UBFL_UniversalSaveLoad
 * @brief A universal Blueprint Function Library for saving and loading USaveGame objects.
 *
 * This class provides static functions for saving and loading USaveGame objects using
 * either fast, compact binary serialization or human-readable JSON serialization.
 */
UCLASS()
class WISPPLUGIN_API UBFL_UniversalSaveLoad : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// --- Binary Save/Load ---

	/**
	 * @brief Saves a USaveGame object to a binary file in the specified slot.
	 * @param SaveGameObject The USaveGame object to save.
	 * @param SlotName The name of the slot to save the game to.
	 * @return True if the game was saved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Universal Save System|Binary", meta = (DisplayName = "Save Game to Binary Slot"))
	static bool SaveGameToBinarySlot(USaveGame* SaveGameObject, const FString& SlotName);

	/**
	 * @brief Loads a USaveGame object from a binary file in the specified slot.
	 * @param SlotName The name of the slot to load the game from.
	 * @return The loaded USaveGame object, or nullptr if the load failed.
	 */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Binary", meta = (DisplayName = "Load Game from Binary Slot"))
	static USaveGame* LoadGameFromBinarySlot(const FString& SlotName);

	/**
	 * @brief Checks if a binary save file exists in the specified slot.
	 * @param SlotName The name of the slot to check.
	 * @return True if a save file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Binary", meta = (DisplayName = "Does Binary Save Exist"))
	static bool DoesBinarySaveExist(const FString& SlotName);

	// --- JSON Save/Load ---

	/**
	 * @brief Saves a USaveGame object to a human-readable JSON file at the specified path.
	 * @param SaveGameObject The USaveGame object to save.
	 * @param FilePath The full path to the file to save the JSON data to.
	 * @return True if the game was saved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Universal Save System|JSON", meta = (DisplayName = "Save Game to JSON File"))
	static bool SaveGameToJsonFile(USaveGame* SaveGameObject, const FString& FilePath);

	/**
	 * @brief Loads a USaveGame object from a JSON file.
	 * @param SaveGameClass The class of the USaveGame object to create.
	 * @param FilePath The full path to the file to load the JSON data from.
	 * @return The loaded USaveGame object, or nullptr if the load failed.
	 */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|JSON", meta = (DisplayName = "Load Game from JSON File"))
	static USaveGame* LoadGameFromJsonFile(TSubclassOf<USaveGame> SaveGameClass, const FString& FilePath);

	// --- Utility ---

	/**
	 * @brief Gets the absolute path to the project's 'Saved' directory.
	 * @return The absolute path to the 'Saved' directory.
	 */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Utility", meta = (DisplayName = "Get Project Save Directory"))
	static FString GetProjectSaveDirectory();
};
