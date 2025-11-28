#include "BFL_GridHelpers.h"

FIntPoint UBFL_GridHelpers::WorldLocationToGridCoordinate(FVector WorldLocation, FVector GridOrigin, float GridCellSize)
{
	if (GridCellSize <= 0.f)
	{
		return FIntPoint::ZeroValue;
	}

	FVector RelativeLocation = WorldLocation - GridOrigin;
	int32 GridX = FMath::FloorToInt(RelativeLocation.X / GridCellSize);
	int32 GridY = FMath::FloorToInt(RelativeLocation.Y / GridCellSize);

	return FIntPoint(GridX, GridY);
}

FVector UBFL_GridHelpers::GridCoordinateToWorldLocation(FIntPoint GridCoordinate, FVector GridOrigin, float GridCellSize)
{
	float WorldX = (GridCoordinate.X * GridCellSize) + (GridCellSize / 2.0f);
	float WorldY = (GridCoordinate.Y * GridCellSize) + (GridCellSize / 2.0f);

	// We return the center of the grid cell.
	return GridOrigin + FVector(WorldX, WorldY, 0);
}
