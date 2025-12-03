// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImportExportDataTable.generated.h"

class UDataTable;

/**
 * @class UImportExportDataTable
 * @brief A Blueprint Function Library for importing and exporting UDataTable assets.
 *
 * This class provides functions for importing and exporting UDataTables from/to JSON or CSV files.
 */
UCLASS()
class WISPPLUGIN_API UImportExportDataTable : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Imports a UDataTable from a JSON or CSV file.
	 * @param SourcePath The source path of the file to import.
	 * @param DestinationPath The destination path in the content browser.
	 * @param StructClass The UScriptStruct to use for the data table rows.
	 * @return The imported UDataTable, or nullptr if the import failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Data Table")
	static UDataTable* ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass);

	/**
	 * @brief Exports a UDataTable to a JSON or CSV file.
	 * @param FilePath The full path to the file to export the data to.
	 * @param DataTable The UDataTable to export.
	 */
	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Data Table")
	static void ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable);
};
