// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonObjectWrapper.h"

#include "MainBlueprintFunctionLibrary.generated.h"


UCLASS()
class UMainBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	//Reads the text file and returns the content of the file
	UFUNCTION(BlueprintCallable, Category = "Dans JSON")
	static FString ReadStringFromFile(FString FilePath, bool& Success, FString& OutInfoMessage);

	//Write the text file
	UFUNCTION(BlueprintCallable, Category = "Dans JSON")
	static void WriteStringToFile(FString FilePath,FString String, bool& Success, FString& OutInfoMessage);

};

