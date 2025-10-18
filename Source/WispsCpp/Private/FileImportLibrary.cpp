// Fill out your copyright notice in the Description page of Project Settings.


#include "FileImportLibrary.h"

#include "Editor/UnrealEd/Public/AssetImportTask.h"
#include "AssetToolsModule.h"

/*
https://www.youtube.com/watch?v=rSV96Ux43dk&t=14s
*/

UAssetImportTask* UFileImportLibrary::CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions, bool& OutSuccess, FString& InfoMessage)
{
	//create Import Task
	UAssetImportTask* RetTask = NewObject<UAssetImportTask>();


	//Sanity Check Make Sure te Object was Created Properly
	if (RetTask == nullptr)
	{
		OutSuccess = false;
		InfoMessage = FString::Printf(TEXT("Create Import Test fail was not ablke to import object, - '%s'"), *SourcePath);
		return nullptr;
	}

	//Set Path Information
	RetTask->Filename = SourcePath;
	RetTask->DestinationPath = FPaths::GetPath(DestinationPath);
	RetTask->DestinationName = FPaths::GetCleanFilename(DestinationPath);


	//Set Basic Options
	// Save the asset once the task is completed
	RetTask->bSave = false;
	//Intrupt flow of the user popups etc? Need to research
	RetTask->bAutomated = true;
	//Is it an aync task?
	RetTask->bAsync = false;
	//Replace the existing asset if it exists
	RetTask->bReplaceExisting = true;
	//Replace its existing settings (unsure whhat settings are om tis context)
	RetTask->bReplaceExistingSettings = false;

	//You can feed these settings into Create import task
	//optional extra factory and options
	RetTask->Factory = ExtraFactory;
	RetTask->Options = ExtraOptions;

	OutSuccess = true;
	InfoMessage = FString::Printf(TEXT("Yay you imported something"), *SourcePath);
	return RetTask;

}

UObject* UFileImportLibrary::ProcessImportTask(UAssetImportTask* ImportTask, bool& OutSucess, FString& InfoMessage)
{
	//Make sure the task is valid we dont want to import something invalid.
	if (ImportTask == nullptr)
	{
		OutSucess = false;
		InfoMessage = FString::Printf(TEXT("ABC"));
		return nullptr;
		//This should prevent from crashing the engine if something invalid slips in
	}

	//Get the Asset Tools Module and set it
	FAssetToolsModule* AssetTools = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools");
	
	//Check to see the Asset tools are not Null
	if (AssetTools == nullptr)
	{
		OutSucess = false;
		InfoMessage = FString::Printf(TEXT("Process Import Task Failed - The AsseteTools Modulke is Invalid"), *ImportTask->Filename);
		return nullptr;
	}

	//Import the asset
	AssetTools->Get().ImportAssetTasks({ ImportTask });

	//Check if anything was imported during the process
	if (ImportTask->GetObjects().Num() == 0)
	{
		OutSucess = false;
		InfoMessage = FString::Printf(TEXT("Process import Task Failed - Nothing was imported . Is your file Valid"), *ImportTask->Filename);
		return nullptr;
	}

	//Because some import tasks actually create mutliple asset (EG Meshes and material) we manually get the right asset
	UObject* ImportedAsset = StaticLoadObject(UObject::StaticClass(), nullptr, *FPaths::Combine( ImportTask->DestinationPath, ImportTask->DestinationName));

	//Return the asset
	OutSucess = true;
	InfoMessage = FString::Printf(TEXT("Process Import Task Succeeded"), *ImportTask->Filename);
	return ImportedAsset;


}

UObject* UFileImportLibrary::ImportAsset(FString SourcePath, FString DestinationPath, bool& OutSuccess, FString& InfoMessage)
{

	//create te import task
	UAssetImportTask* Task = CreateImportTask(SourcePath, DestinationPath, nullptr, nullptr, OutSuccess, InfoMessage);
	if (!OutSuccess)
	{

		return nullptr;
	}

	//import the asset
	UObject* RetAsset = ProcessImportTask(Task, OutSuccess, InfoMessage);
	if (!OutSuccess)
	{

		return nullptr;
	}

	//return the imported asset
	OutSuccess = true;
	InfoMessage = FString::Printf(TEXT("Import Task Succeeded"),*DestinationPath);
	return RetAsset;
}
