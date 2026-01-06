// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "SaveSystem/SovereignSaveManager.h"
#include "SaveSystem/SovereignActorRegistry.h"
#include "Interfaces/SovereignSaveInterface.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Policies/PrettyJsonPrintPolicy.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"


FString USovereignSaveManager::GetSaveFilePath(const FString& SlotName) const
{
	return FPaths::ProjectSavedDir() + TEXT("SaveGames/") + SlotName + TEXT(".json");
}

bool USovereignSaveManager::SaveGame(const FString& SlotName)
{
	USovereignActorRegistry* ActorRegistry = GetWorld()->GetSubsystem<USovereignActorRegistry>();
	if (!ActorRegistry)
	{
		UE_LOG(LogTemp, Error, TEXT("SovereignSaveManager: Cannot find ActorRegistry subsystem."));
		return false;
	}

	TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> ActorsJsonArray;

	for (const auto& Pair : ActorRegistry->GetRegistry())
	{
		AActor* Actor = Pair.Value;
		const FGuid& EntityID = Pair.Key;

		if (Actor && Actor->GetClass()->ImplementsInterface(USovereignSaveInterface::StaticClass()))
		{
			TSharedPtr<FJsonObject> ActorJsonObject = MakeShareable(new FJsonObject());
			ActorJsonObject->SetStringField(TEXT("EntityID"), EntityID.ToString());
			ActorJsonObject->SetStringField(TEXT("ActorClass"), Actor->GetClass()->GetPathName());
			ActorJsonObject->SetStringField(TEXT("ActorTransform"), Actor->GetTransform().ToString());

			TMap<FName, FString> SaveData = ISovereignSaveInterface::Execute_GetSaveData(Actor);
			TSharedPtr<FJsonObject> CustomDataJsonObject = MakeShareable(new FJsonObject());
			for (const auto& DataPair : SaveData)
			{
				CustomDataJsonObject->SetStringField(DataPair.Key.ToString(), DataPair.Value);
			}
			ActorJsonObject->SetObjectField(TEXT("CustomData"), CustomDataJsonObject);

			ActorsJsonArray.Add(MakeShareable(new FJsonValueObject(ActorJsonObject)));
		}
	}

	RootJsonObject->SetArrayField(TEXT("SavedActors"), ActorsJsonArray);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(RootJsonObject.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *GetSaveFilePath(SlotName));
}

bool USovereignSaveManager::LoadGame(const FString& SlotName)
{
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *GetSaveFilePath(SlotName)))
	{
		UE_LOG(LogTemp, Error, TEXT("SovereignSaveManager: Failed to load save file: %s"), *SlotName);
		return false;
	}

	TSharedPtr<FJsonObject> RootJsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, RootJsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("SovereignSaveManager: Failed to parse JSON from save file: %s"), *SlotName);
		return false;
	}

    // Clear existing actors from registry before loading
    USovereignActorRegistry* ActorRegistry = GetWorld()->GetSubsystem<USovereignActorRegistry>();
    if (!ActorRegistry)
	{
		UE_LOG(LogTemp, Error, TEXT("SovereignSaveManager: Cannot find ActorRegistry subsystem during load."));
		return false;
	}

    for (const auto& Pair : ActorRegistry->GetRegistry())
    {
        if(Pair.Value)
        {
            Pair.Value->Destroy();
        }
    }

	const TArray<TSharedPtr<FJsonValue>>* ActorsJsonArray;
	if (RootJsonObject->TryGetArrayField(TEXT("SavedActors"), ActorsJsonArray))
	{
		for (const auto& ActorValue : *ActorsJsonArray)
		{
			const TSharedPtr<FJsonObject> ActorJsonObject = ActorValue->AsObject();
			FGuid EntityID;
			FGuid::Parse(ActorJsonObject->GetStringField(TEXT("EntityID")), EntityID);

			FString ActorClassPath = ActorJsonObject->GetStringField(TEXT("ActorClass"));
			UClass* ActorClass = FindObject<UClass>(ANY_PACKAGE, *ActorClassPath);
			if (!ActorClass)
			{
				UE_LOG(LogTemp, Warning, TEXT("SovereignSaveManager: Could not find class %s"), *ActorClassPath);
				continue;
			}

			FTransform ActorTransform;
			ActorTransform.InitFromString(ActorJsonObject->GetStringField(TEXT("ActorTransform")));

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, ActorTransform, SpawnParams);

			if (SpawnedActor)
			{
                ActorRegistry->RegisterActor(SpawnedActor, EntityID);

				const TSharedPtr<FJsonObject>* CustomDataObject;
				if (ActorJsonObject->TryGetObjectField(TEXT("CustomData"), CustomDataObject))
				{
					TMap<FName, FString> CustomDataMap;
					for (const auto& Pair : (*CustomDataObject)->Values)
					{
						CustomDataMap.Add(FName(*Pair.Key), Pair.Value->AsString());
					}

					if (SpawnedActor->GetClass()->ImplementsInterface(USovereignSaveInterface::StaticClass()))
					{
						ISovereignSaveInterface::Execute_LoadSaveData(SpawnedActor, CustomDataMap);
					}
				}
			}
		}
	}

	return true;
}
