// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "GridDataComponent.generated.h"

/**
 * @class UGridDataComponent
 * @brief An actor component that holds the data for a grid.
 *
 * This component stores the soil data for each cell in a grid, as well as the
 * grid's cell size and origin.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UGridDataComponent();

	/**
	 * @brief The main TMap holding the data for every cell in the grid.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TMap<FIntPoint, FSoilData> SoilDataGrid;

	/**
	 * @brief The size of each square cell in world units (cm).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float GridCellSize = 100.0f;

	/**
	 * @brief The world location that corresponds to grid cell (0,0).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector GridOrigin = FVector::ZeroVector;
};
