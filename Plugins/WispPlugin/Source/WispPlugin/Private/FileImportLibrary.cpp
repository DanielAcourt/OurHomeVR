// Copyright Epic Games, Inc. All Rights Reserved.

#include "FileImportLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/Factory.h"
#include "AssetImportTask.h"

UAssetImportTask* UFileImportLibrary::CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions)
{
	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->bSave = true;
	ImportTask->bAutomated = true;
	ImportTask->DestinationName = TEXT("");
	ImportTask->DestinationPath = DestinationPath;
	ImportTask->Filename = SourcePath;
	ImportTask->Factory = ExtraFactory;
	ImportTask->Options = ExtraOptions;
	return ImportTask;
}

UObject* UFileImportLibrary::ProcessImportTask(UAssetImportTask* ImportTask)
{
	if (ImportTask == nullptr)
	{
		return nullptr;
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });

	if (ImportTask->Result.Num() > 0)
	{
		return ImportTask->Result[0];
	}

	return nullptr;
}

UObject* UFileImportLibrary::ImportAsset(FString SourcePath, FString DestinationPath)
{
	UAssetImportTask* Task = CreateImportTask(SourcePath, DestinationPath, nullptr, nullptr);
	return ProcessImportTask(Task);
}
