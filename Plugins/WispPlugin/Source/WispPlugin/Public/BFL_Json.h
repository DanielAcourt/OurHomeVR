#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GardenData.h" // Include our new data definitions


#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

class FJsonObject;

#include "BFL_Json.generated.h"



UCLASS()
class WISPPLUGIN_API UBFL_Json : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Garden Save System")
	static void SaveGameData(FString JsonFilePath, const FSaveGameData& SaveData, bool& OutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintPure, Category = "Garden Save System")
	static FSaveGameData LoadGameData(FString JsonFilePath, bool& OutSuccess, FString& OutInfoMessage);

public:
	// These helper functions are not exposed to Blueprints but are used by the functions above.
	static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath, bool& OutSuccess, FString& OutInfoMessage);
	static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& OutSuccess, FString& OutInfoMessage);
};
