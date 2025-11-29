// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImportExportDataTable.generated.h"

class UDataTable;

UCLASS()
class WISPPLUGIN_API UImportExportDataTable : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Data Table")
	static UDataTable* ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Data Table")
	static void ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable);
};
