// Fill out your copyright notice in the Description page of Project Settings.


#include "ImportExportDataTable.h"
#include "MainBlueprintFunctionLibrary.h"
#include "BFL_Json.h"
#include "FileImportLibrary.h"

#include "Factories/ReimportDataTableFactory.h" //Unreal editor only

UDataTable* UImportExportDataTable::ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, bool& OutSuccess, FString& InfoMessage)
{
	UReimportDataTableFactory* DtFactory = NewObject<UReimportDataTableFactory>();
	DtFactory->AutomatedImportSettings.ImportType = ECSVImportType::ECSV_DataTable;
	DtFactory->AutomatedImportSettings.ImportRowStruct = StructClass;

	//Create Import Task
	UAssetImportTask* ImportTask = UFileImportLibrary::CreateImportTask(SourcePath, DestinationPath, DtFactory, nullptr, OutSuccess, InfoMessage);
	if (!OutSuccess)
	{
		return nullptr;
	}

	UObject* ImportedAsset = UFileImportLibrary::ProcessImportTask(ImportTask, OutSuccess, InfoMessage);
	if(!OutSuccess)
	{
		return nullptr;
	}

	//Return Imported Asset
	OutSuccess = true;

	//InfoMessage = FString::Printf(TEXT("Succeed 1"), DestinationPath);
	return Cast<UDataTable>(ImportedAsset);

	
}

void UImportExportDataTable::ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable, bool& OutSuccess, FString& InfoMessage)
{
	if (DataTable == nullptr)
	{
		OutSuccess = false;
		//InfoMessage = FString::Printf(TEXT("Failed 1"), FilePath);
		return;

	}

	FString TableString;

	if (FilePath.Contains(".csv"))
	{
		TableString = DataTable->GetTableAsCSV();

	}
	else 
	{
		TableString = DataTable->GetTableAsJSON();
	}

	//Write text file
	UMainBlueprintFunctionLibrary::WriteStringToFile(FilePath, TableString, OutSuccess, InfoMessage);
}
