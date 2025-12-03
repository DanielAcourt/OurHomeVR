// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_WispSaveManager.generated.h"

class UWispSaveGame;
class AGardenGridManager;

/**
 * @class UBFL_WispSaveManager
 * @brief A Blueprint Function Library for managing game save and load operations.
 *
 * This class provides functions to create a save object from the current world state
 * and to load a world state from a save object.
 */
UCLASS()
class WISPPLUGIN_API UBFL_WispSaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates a save object from the current state of the world.
	 * @param WorldContextObject A UObject to get the world from.
	 * @return A new UWispSaveGame object containing the saved world state.
	 */
	UFUNCTION(BlueprintPure, Category = "Wisp Save Manager")
	static UWispSaveGame* CreateSaveObjectFromWorld(const UObject* WorldContextObject);

	/**
	 * @brief Loads the world state from a given save object.
	 * @param WorldContextObject A UObject to get the world from.
	 * @param SaveGame The UWispSaveGame object to load the world from.
	 */
	UFUNCTION(BlueprintCallable, Category = "Wisp Save Manager")
	static void LoadWorldFromSaveObject(const UObject* WorldContextObject, UWispSaveGame* SaveGame);
};
