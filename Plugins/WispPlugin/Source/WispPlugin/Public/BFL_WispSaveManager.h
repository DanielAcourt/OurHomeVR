// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_WispSaveManager.generated.h"

class UWispSaveGame;
class AGardenGridManager;

UCLASS()
class WISPPLUGIN_API UBFL_WispSaveManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Wisp Save Manager")
	static UWispSaveGame* CreateSaveObjectFromWorld(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Wisp Save Manager")
	static void LoadWorldFromSaveObject(const UObject* WorldContextObject, UWispSaveGame* SaveGame);
};
