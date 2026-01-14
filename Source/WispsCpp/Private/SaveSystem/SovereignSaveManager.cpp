// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "SaveSystem/SovereignSaveManager.h"
#include "SaveSystem/SovereignSaveGame.h"
#include "SaveSystem/SovereignGameData.h"
#include "SaveSystem/SovereignActorRegistry.h"
#include "Entities/SovereignSaveableEntityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h" // Requires "JsonUtilities" in Build.cs
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"


void USovereignSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("SovereignSaveManager: Initialized."));
}

void USovereignSaveManager::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("SovereignSaveManager: Shutting down."));
    Super::Deinitialize();
}

void USovereignSaveManager::SaveWorldState(const FString& SlotName, bool bAsJson)
{
    UWorld* World = GetWorld();
    if (!World) return;

    USovereignActorRegistry* Registry = World->GetSubsystem<USovereignActorRegistry>();
    USovereignSaveGame* SaveSuitcase = Cast<USovereignSaveGame>(UGameplayStatics::CreateSaveGameObject(USovereignSaveGame::StaticClass()));

    if (!Registry || !SaveSuitcase)
    {
        UE_LOG(LogTemp, Error, TEXT("SovereignSaveManager: Could not get ActorRegistry or create SaveGame object. Save failed for slot '%s'."), *SlotName);
        return;
    }

    // Iterate through all registered actors and gather their data via their "Passport" component.
    for (const auto& Elem : Registry->GetActiveRegistry())
    {
        if (AActor* TargetActor = Elem.Value.Get())
        {
            if (auto* SaveComp = TargetActor->FindComponentByClass<USovereignSaveableEntityComponent>())
            {
                FEntitySaveData Data;

                // --- 1. IDENTITY ---
                Data.MyGUID = SaveComp->EntityID;
                Data.ClassPath = TargetActor->GetClass()->GetPathName();

                // --- 2. PHYSICALITY ---
                Data.WorldTransform = TargetActor->GetActorTransform();

                // --- 3. LINEAGE ---
                Data.ParentID = SaveComp->ParentID;
                Data.MotherID = SaveComp->MotherID;
                Data.FatherID = SaveComp->FatherID;
                Data.bIsFemale = SaveComp->bIsFemale;
                Data.OffspringCount = SaveComp->OffspringCount;
                Data.MatingHistory = SaveComp->MatingHistory;

                // --- 4. STATE (The Master Scrape) ---
                // This captures all other data from the component itself (e.g., elements, maturity)
                // and any other component that implements the save interface.
                Data.UnknownMetaTags = SaveComp->CaptureFullEntityState();

                SaveSuitcase->SavedActors.Add(Data);
            }
        }
    }

    // --- FINAL COMMIT ---
    SaveSuitcase->SaveSlotName = SlotName;
    SaveSuitcase->SaveTime = FDateTime::UtcNow();

    if (bAsJson)
    {
        const FString JsonStr = ConvertSuitcaseToJson(SaveSuitcase);
        const FString FullPath = GetSaveFilePath(SlotName);

        if (FFileHelper::SaveStringToFile(JsonStr, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8))
        {
             UE_LOG(LogTemp, Log, TEXT("Sovereign: World State committed to JSON file %s. Total Entities: %d"), *FullPath, SaveSuitcase->SavedActors.Num());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Sovereign: Failed to write JSON to file %s."), *FullPath);
        }
    }
    else
    {
        UGameplayStatics::SaveGameToSlot(SaveSuitcase, SlotName, 0);
        UE_LOG(LogTemp, Log, TEXT("Sovereign: World State committed to binary slot %s. Total Entities: %d"), *SlotName, SaveSuitcase->SavedActors.Num());
    }
}

void USovereignSaveManager::LoadWorldState(const FString& SlotName, bool bAsJson)
{
    USovereignSaveGame* LoadedSuitcase = nullptr;

    if (bAsJson)
    {
        FString JsonContent;
        if (FFileHelper::LoadFileToString(JsonContent, *GetSaveFilePath(SlotName)))
        {
            LoadedSuitcase = ConvertJsonToSuitcase(JsonContent);
        }
    }
    else
    {
        LoadedSuitcase = Cast<USovereignSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    }

    if (!LoadedSuitcase)
    {
        UE_LOG(LogTemp, Error, TEXT("Sovereign: Failed to load save data for slot '%s'."), *SlotName);
        return;
    }

    // Phase 1: Clean the world of any entity not present in the save file.
    PurgeUnearnedEntities(LoadedSuitcase);

    UWorld* World = GetWorld();
    USovereignActorRegistry* Registry = World ? World->GetSubsystem<USovereignActorRegistry>() : nullptr;
    if (!World || !Registry) return;

    // Phase 2: Reconstitute the world from the save data.
    for (const FEntitySaveData& Data : LoadedSuitcase->SavedActors)
    {
        AActor* TargetActor = Registry->FindActor(Data.MyGUID);

        // Spawn the actor if it doesn't exist.
        if (!TargetActor)
        {
            UClass* ActorClass = LoadObject<UClass>(nullptr, *Data.ClassPath);
            if (ActorClass)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                TargetActor = World->SpawnActor<AActor>(ActorClass, Data.WorldTransform, SpawnParams);
            }
        }

        // Phase 3. Apply State: Update physical and metabolic data for all actors (newly spawned or existing).
        if (TargetActor)
        {
            TargetActor->SetActorTransform(Data.WorldTransform);
            if (auto* SaveComp = TargetActor->FindComponentByClass<USovereignSaveableEntityComponent>())
            {
                // Restore Identity & Lineage
                SaveComp->EntityID = Data.MyGUID;
                SaveComp->ParentID = Data.ParentID;
                SaveComp->MotherID = Data.MotherID;
                SaveComp->FatherID = Data.FatherID;
                SaveComp->bIsFemale = Data.bIsFemale;
                SaveComp->OffspringCount = Data.OffspringCount;
                SaveComp->MatingHistory = Data.MatingHistory;

                // Restore all other component and meta tag data
                SaveComp->ApplyMetaTags(Data.UnknownMetaTags);
            }
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Sovereign: World synchronized with save slot '%s'."), *SlotName);
}

void USovereignSaveManager::PurgeUnearnedEntities(USovereignSaveGame* LoadedSuitcase)
{
    if (!LoadedSuitcase) return;

    UWorld* World = GetWorld();
    USovereignActorRegistry* Registry = World ? World->GetSubsystem<USovereignActorRegistry>() : nullptr;
    if (!Registry) return;

    TArray<AActor*> TrackedActors = Registry->GetTrackedActorsAsList();

    for (AActor* Actor : TrackedActors)
    {
        auto* SaveComp = Actor->FindComponentByClass<USovereignSaveableEntityComponent>();
        if (!SaveComp) continue;

        const bool bFoundInSave = LoadedSuitcase->SavedActors.ContainsByPredicate(
            [&](const FEntitySaveData& SavedData) {
                return SavedData.MyGUID == SaveComp->EntityID;
            });

        if (!bFoundInSave)
        {
            UE_LOG(LogTemp, Warning, TEXT("Sovereign: Purging unearned entity %s (ID: %s)."), *Actor->GetName(), *SaveComp->EntityID.ToString());
            Actor->Destroy(); // Registry unregistration happens automatically in the component's EndPlay.
        }
    }
}

int32 USovereignSaveManager::GetSavedEntityCount(const FString& SlotName, bool bIsJson)
{
    USovereignSaveGame* TempSuitcase = nullptr;

    if (bIsJson)
    {
        FString JsonContent;
        if (FFileHelper::LoadFileToString(JsonContent, *GetSaveFilePath(SlotName)))
        {
            TempSuitcase = ConvertJsonToSuitcase(JsonContent);
        }
    }
    else
    {
        TempSuitcase = Cast<USovereignSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    }

    return (TempSuitcase) ? TempSuitcase->SavedActors.Num() : 0;
}

FString USovereignSaveManager::ConvertSuitcaseToJson(USovereignSaveGame* Suitcase)
{
    if (!Suitcase) return TEXT("");

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);

    Writer->WriteObjectStart();
    Writer->WriteArrayStart(TEXT("SavedActors"));
    for (const FEntitySaveData& Data : Suitcase->SavedActors)
    {
        TSharedPtr<FJsonObject> DataObject;
        FJsonObjectConverter::UStructToJsonObject(FEntitySaveData::StaticStruct(), &Data, DataObject, 0, 0);

        TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(DataObject));
        Writer->WriteValue(JsonValue);
    }
    Writer->WriteArrayEnd();
    Writer->WriteObjectEnd();
    Writer->Close();

    return OutputString;
}

USovereignSaveGame* USovereignSaveManager::ConvertJsonToSuitcase(const FString& JsonContent)
{
    USovereignSaveGame* NewSuitcase = Cast<USovereignSaveGame>(UGameplayStatics::CreateSaveGameObject(USovereignSaveGame::StaticClass()));
    if (!NewSuitcase) return nullptr;

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        return nullptr;
    }

    const TArray<TSharedPtr<FJsonValue>>* SavedActorsArray;
    if (RootObject->TryGetArrayField(TEXT("SavedActors"), SavedActorsArray))
    {
        for (const TSharedPtr<FJsonValue>& Value : *SavedActorsArray)
        {
            FEntitySaveData Data;
            if (FJsonObjectConverter::JsonAttributesToUStruct(Value->AsObject()->Values, FEntitySaveData::StaticStruct(), &Data, 0, 0))
            {
                NewSuitcase->SavedActors.Add(Data);
            }
        }
    }

    return NewSuitcase;
}

FString USovereignSaveManager::GetSaveFilePath(const FString& SlotName) const
{
	FString CleanFileName = SlotName;
	CleanFileName.ReplaceInline(TEXT("/"), TEXT("_"));
	CleanFileName.ReplaceInline(TEXT("\\"), TEXT("_"));
	CleanFileName.ReplaceInline(TEXT(" "), TEXT("_"));

	if (!CleanFileName.EndsWith(".json"))
	{
		CleanFileName += ".json";
	}

    const FString SaveDir = FPaths::ProjectSavedDir() + TEXT("SovereignSaves/");

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*SaveDir))
	{
		PlatformFile.CreateDirectoryTree(*SaveDir);
	}

	return SaveDir + CleanFileName;
}
