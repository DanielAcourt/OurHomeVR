// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h" // Our FSoilData struct is in here
#include "GridDataComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGridDataComponent();

	// The main TMap holding the data for every cell in the grid.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TMap<FIntPoint, FSoilData> SoilDataGrid;

	// The size of each square cell in world units (cm).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float GridCellSize = 100.0f;

	// The world location that corresponds to grid cell (0,0).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector GridOrigin = FVector::ZeroVector;
};