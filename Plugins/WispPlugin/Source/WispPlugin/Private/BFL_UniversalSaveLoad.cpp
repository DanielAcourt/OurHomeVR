// Copyright Epic Games, Inc. All Rights Reserved.

#include "BFL_UniversalSaveLoad.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "JsonObjectConverter.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UBFL_UniversalSaveLoad::SaveGameToBinarySlot(USaveGame* SaveGameObject, const FString& SlotName)
{
	if (SaveGameObject == nullptr)
	{
		return false;
	}

	return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

USaveGame* UBFL_UniversalSaveLoad::LoadGameFromBinarySlot(const FString& SlotName)
{
	return UGameplayStatics::LoadGameFromSlot(SlotName, 0);
}

bool UBFL_UniversalSaveLoad::DoesBinarySaveExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool UBFL_UniversalSaveLoad::SaveGameToJsonFile(USaveGame* SaveGameObject, const FString& FilePath)
{
	if (SaveGameObject == nullptr)
	{
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	if (!FJsonObjectConverter::UStructToJsonObject(SaveGameObject->GetClass(), SaveGameObject, JsonObject.ToSharedRef(), 0, 0))
	{
		return false;
	}

	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return FFileHelper::SaveStringToFile(JsonString, *FilePath);
}

USaveGame* UBFL_UniversalSaveLoad::LoadGameFromJsonFile(TSubclassOf<USaveGame> SaveGameClass, const FString& FilePath)
{
	if (SaveGameClass == nullptr)
	{
		return nullptr;
	}

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		return nullptr;
	}

	USaveGame* LoadedGame = NewObject<USaveGame>(GetTransientPackage(), SaveGameClass);
	if (LoadedGame == nullptr)
	{
		return nullptr;
	}

	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), LoadedGame->GetClass(), LoadedGame, 0, 0))
	{
		return nullptr;
	}

	return LoadedGame;
}

FString UBFL_UniversalSaveLoad::GetProjectSaveDirectory()
{
	return FPaths::ProjectSavedDir();
}
