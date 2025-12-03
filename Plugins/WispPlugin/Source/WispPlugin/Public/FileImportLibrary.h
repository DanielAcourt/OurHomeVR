// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileImportLibrary.generated.h"

class UAssetImportTask;
class UFactory;

/**
 * @class UFileImportLibrary
 * @brief A Blueprint Function Library for importing assets into the editor.
 *
 * This class provides functions for creating and processing asset import tasks.
 */
UCLASS()
class WISPPLUGIN_API UFileImportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates an asset import task.
	 * @param SourcePath The source path of the file to import.
	 * @param DestinationPath The destination path in the content browser.
	 * @param ExtraFactory An optional factory to use for importing the asset.
	 * @param ExtraOptions Optional extra options for the import task.
	 * @return The created UAssetImportTask object.
	 */
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UAssetImportTask* CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions);

	/**
	 * @brief Processes an asset import task.
	 * @param ImportTask The import task to process.
	 * @return The imported UObject, or nullptr if the import failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UObject* ProcessImportTask(UAssetImportTask* ImportTask);

	/**
	 * @brief Imports an asset directly.
	 * @param SourcePath The source path of the file to import.
	 * @param DestinationPath The destination path in the content browser.
	 * @return The imported UObject, or nullptr if the import failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UObject* ImportAsset(FString SourcePath, FString DestinationPath);
};
