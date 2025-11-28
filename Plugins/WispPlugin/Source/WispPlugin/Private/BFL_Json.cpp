// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL_Json.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"


void UBFL_Json::SaveGameData(FString JsonFilePath, const FSaveGameData& SaveData, bool& OutSuccess, FString& OutInfoMessage)
{
    // Convert the FSaveGameData struct to a FJsonObject
    TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(SaveData);

    if (!JsonObject.IsValid())
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("SaveGameData Failed - Could not convert SaveData struct to a Json Object."));
        return;
    }

    // Write the JsonObject to the file
    WriteJson(JsonFilePath, JsonObject, OutSuccess, OutInfoMessage);

}

FSaveGameData UBFL_Json::LoadGameData(FString JsonFilePath, bool& OutSuccess, FString& OutInfoMessage)
{
    FSaveGameData LoadedData;

    // Read the json file into a FJsonObject
    TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, OutSuccess, OutInfoMessage);

    if (!OutSuccess)
    {
        // ReadJson will have set the OutInfoMessage
        return LoadedData;
    }

    // Convert the FJsonObject to the FSaveGameData struct
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LoadedData))
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("LoadGameData Failed - Could not convert Json Object to SaveData struct from file: %s"), *JsonFilePath);
        return LoadedData;
    }

    OutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("LoadGameData Succeeded - Loaded data from: %s"), *JsonFilePath);
    return LoadedData;
}


TSharedPtr<FJsonObject> UBFL_Json::ReadJson(FString JsonFilePath, bool& OutSuccess, FString& OutInfoMessage)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *JsonFilePath))
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("ReadJson Failed - Could not read file: %s"), *JsonFilePath);
        return nullptr;
    }

    TSharedPtr<FJsonObject> RetJsonObject;
    if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("ReadJson Failed - Could not deserialize Json string from file: %s. Is it valid JSON?"), *JsonFilePath);
        return nullptr;
    }

    OutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("ReadJson Succeeded - Read file: %s"), *JsonFilePath);
    return RetJsonObject;
}

void UBFL_Json::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& OutSuccess, FString& OutInfoMessage)
{
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("WriteJson Failed - Could not serialize Json object to string."));
        return;
    }

    if (!FFileHelper::SaveStringToFile(JsonString, *JsonFilePath))
    {
        OutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("WriteJson Failed - Could not write to file: %s"), *JsonFilePath);
        return;
    }

    OutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("WriteJson Succeeded - Wrote to file: %s"), *JsonFilePath);
}
