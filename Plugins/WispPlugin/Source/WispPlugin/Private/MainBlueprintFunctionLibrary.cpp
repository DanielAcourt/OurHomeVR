// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainBlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"

bool UMainBlueprintFunctionLibrary::LoadTextFromFile(FString FilePath, FString& OutText)
{
	return FFileHelper::LoadFileToString(OutText, *FilePath);
}

bool UMainBlueprintFunctionLibrary::SaveTextToFile(FString FilePath, FString Text)
{
	return FFileHelper::SaveStringToFile(Text, *FilePath);
}
