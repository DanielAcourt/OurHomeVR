// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImportExportDataTable.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "AssetToolsModule.h"
#include "Factories/CsvImportFactory.h"

UDataTable* UImportExportDataTable::ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass)
{
	if (StructClass == nullptr)
	{
		return nullptr;
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	// For CSV, we can use the built-in factory
	if (SourcePath.EndsWith(TEXT(".csv")))
	{
		UCsvImportFactory* Factory = NewObject<UCsvImportFactory>();
		Factory->AutomatedImportSettings.ImportRowStruct = StructClass;

		UObject* ImportedObject = AssetToolsModule.Get().ImportAssets(SourcePath, DestinationPath, Factory).Pop();
		return Cast<UDataTable>(ImportedObject);
	}

	// For JSON, it's a bit more manual as there's no direct factory
	if (SourcePath.EndsWith(TEXT(".json")))
	{
		FString FileContents;
		if (FFileHelper::LoadFileToString(FileContents, *SourcePath))
		{
			FString PackageName = DestinationPath + TEXT("/") + FPaths::GetBaseFilename(SourcePath);
			UPackage* Package = CreatePackage(*PackageName);
			UDataTable* DataTable = NewObject<UDataTable>(Package, FName(*FPaths::GetBaseFilename(SourcePath)), RF_Public | RF_Standalone);
			DataTable->RowStruct = StructClass;
			DataTable->CreateTableFromJSONString(FileContents);

			if (DataTable)
			{
				FAssetRegistryModule::AssetCreated(DataTable);
				DataTable->MarkPackageDirty();
				return DataTable;
			}
		}
	}

	return nullptr;
}

void UImportExportDataTable::ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable)
{
	if (DataTable == nullptr)
	{
		return;
	}

	FString OutputString;
	if (FilePath.EndsWith(TEXT(".csv")))
	{
		OutputString = DataTable->GetTableAsCSV();
	}
	else if (FilePath.EndsWith(TEXT(".json")))
	{
		OutputString = DataTable->GetTableAsJSON();
	}

	FFileHelper::SaveStringToFile(OutputString, *FilePath);
}
