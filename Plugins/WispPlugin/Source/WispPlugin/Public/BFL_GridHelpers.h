// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GridHelpers.generated.h"

/**
 * @class UBFL_GridHelpers
 * @brief A library of helper functions for grid-based calculations.
 *
 * This class provides static functions for converting between world locations and grid coordinates.
 */
UCLASS()
class WISPPLUGIN_API UBFL_GridHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Converts a world location to a grid coordinate.
	 * @param WorldLocation The world location to convert.
	 * @param GridOrigin The origin of the grid in world space.
	 * @param GridCellSize The size of each cell in the grid.
	 * @return The corresponding grid coordinate.
	 */
	UFUNCTION(BlueprintPure, Category = "Garden Grid")
	static FIntPoint WorldLocationToGridCoordinate(FVector WorldLocation, FVector GridOrigin, float GridCellSize);

	/**
	 * @brief Converts a grid coordinate to a world location.
	 * @param GridCoordinate The grid coordinate to convert.
	 * @param GridOrigin The origin of the grid in world space.
	 * @param GridCellSize The size of each cell in the grid.
	 * @return The corresponding world location.
	 */
	UFUNCTION(BlueprintPure, Category = "Garden Grid")
	static FVector GridCoordinateToWorldLocation(FIntPoint GridCoordinate, FVector GridOrigin, float GridCellSize);
};
