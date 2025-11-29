// Copyright Epic Games, Inc. All Rights Reserved.

#include "BFL_GridHelpers.h"

FIntPoint UBFL_GridHelpers::WorldLocationToGridCoordinate(FVector WorldLocation, FVector GridOrigin, float GridCellSize)
{
	if (GridCellSize <= 0)
	{
		return FIntPoint::NoneValue;
	}

	FVector RelativeLocation = WorldLocation - GridOrigin;
	int32 X = FMath::RoundToInt(RelativeLocation.X / GridCellSize);
	int32 Y = FMath::RoundToInt(RelativeLocation.Y / GridCellSize);
	return FIntPoint(X, Y);
}

FVector UBFL_GridHelpers::GridCoordinateToWorldLocation(FIntPoint GridCoordinate, FVector GridOrigin, float GridCellSize)
{
	float X = GridCoordinate.X * GridCellSize;
	float Y = GridCoordinate.Y * GridCellSize;
	return FVector(X, Y, 0) + GridOrigin;
}
