// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainBlueprintFunctionLibrary.generated.h"

/**
 * @class UMainBlueprintFunctionLibrary
 * @brief A library of general-purpose Blueprint functions.
 *
 * This class provides static functions for file I/O operations.
 */
UCLASS()
class WISPPLUGIN_API UMainBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Loads text from a file.
	 * @param FilePath The full path to the file to load.
	 * @param OutText The loaded text.
	 * @return True if the text was loaded successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "File IO")
	static bool LoadTextFromFile(FString FilePath, FString& OutText);

	/**
	 * @brief Saves text to a file.
	 * @param FilePath The full path to the file to save to.
	 * @param Text The text to save.
	 * @return True if the text was saved successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool SaveTextToFile(FString FilePath, FString Text);
};
