// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_UniversalSaveLoad.generated.h"

class USaveGame;

/**
 * A universal Blueprint Function Library for saving and loading USaveGame objects.
 * Supports both fast, compact binary serialization and human-readable JSON serialization.
 */
UCLASS()
class WISPPLUGIN_API UBFL_UniversalSaveLoad : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// --- Binary Save/Load ---

	/** Saves a USaveGame object to a binary file in the specified slot. */
	UFUNCTION(BlueprintCallable, Category = "Universal Save System|Binary", meta = (DisplayName = "Save Game to Binary Slot"))
	static bool SaveGameToBinarySlot(USaveGame* SaveGameObject, const FString& SlotName);

	/** Loads a USaveGame object from a binary file in the specified slot. */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Binary", meta = (DisplayName = "Load Game from Binary Slot"))
	static USaveGame* LoadGameFromBinarySlot(const FString& SlotName);

	/** Checks if a binary save file exists in the specified slot. */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Binary", meta = (DisplayName = "Does Binary Save Exist"))
	static bool DoesBinarySaveExist(const FString& SlotName);

	// --- JSON Save/Load ---

	/** Saves a USaveGame object to a human-readable JSON file at the specified path. */
	UFUNCTION(BlueprintCallable, Category = "Universal Save System|JSON", meta = (DisplayName = "Save Game to JSON File"))
	static bool SaveGameToJsonFile(USaveGame* SaveGameObject, const FString& FilePath);

	/** Loads a USaveGame object from a JSON file. The user must provide the class of the USaveGame object to create. */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|JSON", meta = (DisplayName = "Load Game from JSON File"))
	static USaveGame* LoadGameFromJsonFile(TSubclassOf<USaveGame> SaveGameClass, const FString& FilePath);

	// --- Utility ---

	/** Gets the absolute path to the project's 'Saved' directory. A good place to store JSON save files. */
	UFUNCTION(BlueprintPure, Category = "Universal Save System|Utility", meta = (DisplayName = "Get Project Save Directory"))
	static FString GetProjectSaveDirectory();
};
