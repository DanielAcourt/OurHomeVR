// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImportExportDataTable.generated.h"

/**
 * 
 */
UCLASS()
class WISPSCPP_API UImportExportDataTable : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	* Editor Only
	* https://www.youtube.com/watch?v=qO-5PAae5IE
	* 
	@Param SourcePath
	@Param DesitinationPath
	@ Param StructClass
	@Param OutSuccess
	@Param InfoMessage

	@Return the imported datatable
	
	*/

	UFUNCTION(BlueprintCallable, Category = "Dans Json")
	static UDataTable* ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, bool& OutSuccess, FString& InfoMessage);

	/*
	Editor Only
	*/

	UFUNCTION(BlueprintCallable, Category = "Dans Json")
	static void ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable, bool& OutSuccess, FString& InfoMessage);

};
