// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileImportLibrary.generated.h"

class UAssetImportTask;
class UFactory;

UCLASS()
class WISPPLUGIN_API UFileImportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UAssetImportTask* CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UObject* ProcessImportTask(UAssetImportTask* ImportTask);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | File Import")
	static UObject* ImportAsset(FString SourcePath, FString DestinationPath);
};
