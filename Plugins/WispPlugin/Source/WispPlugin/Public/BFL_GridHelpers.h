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

/*
How to Use This
1. After compiling, create a BP_GardenGridManager actor.

2. Add the new GridDataComponent to it. You can configure the GridCellSize and GridOrigin here.

3.From any other Blueprint (like your InteractionComponent), you can now call the WorldLocationToGridCoordinate
helper function to get a cell coordinate, and then use that to Find data in the SoilDataGrid on the manager's component.

*/