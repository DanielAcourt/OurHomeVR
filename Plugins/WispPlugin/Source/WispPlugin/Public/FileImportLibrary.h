// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileImportLibrary.generated.h"

/**
 * 
 */

class UAssetImportTask;
class UFactory;

UCLASS()
class WISPPLUGIN_API UFileImportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	Notes
	@Param Source Path
	@Param DesitinationPath
	@Param Extra Factor
	@Param Extrea Options
	
	*/
	
	UFUNCTION(BlueprintCallable, Category = "Dans Json")
		static UAssetImportTask* CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions, bool& OutSuccess, FString& InfoMessage);

	UFUNCTION(BlueprintCallable, Category = "Dans Json")
		static UObject* ProcessImportTask(UAssetImportTask* ImportTask, bool& OutSucess, FString& InfoMessage);
	
	UFUNCTION(BlueprintCallable, Category = "Dans Json")
		static UObject* ImportAsset(FString SourcePath, FString DestinationPath, bool& OutSuccess, FString& InfoMessage);
};
