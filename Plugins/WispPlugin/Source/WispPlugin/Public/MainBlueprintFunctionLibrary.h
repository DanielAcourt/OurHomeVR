// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainBlueprintFunctionLibrary.generated.h"

UCLASS()
class WISPPLUGIN_API UMainBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "File IO")
	static bool LoadTextFromFile(FString FilePath, FString& OutText);

	UFUNCTION(BlueprintCallable, Category = "File IO")
	static bool SaveTextToFile(FString FilePath, FString Text);
};
