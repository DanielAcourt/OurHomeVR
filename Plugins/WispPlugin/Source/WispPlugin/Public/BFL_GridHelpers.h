// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GridHelpers.generated.h"

UCLASS()
class WISPPLUGIN_API UBFL_GridHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Garden Grid")
	static FIntPoint WorldLocationToGridCoordinate(FVector WorldLocation, FVector GridOrigin, float GridCellSize);

	UFUNCTION(BlueprintPure, Category = "Garden Grid")
	static FVector GridCoordinateToWorldLocation(FIntPoint GridCoordinate, FVector GridOrigin, float GridCellSize);
};
