// Copyright Epic Games, Inc. All Rights Reserved.

#include "GardenGridManager.h"
#include "GridDataComponent.h"

AGardenGridManager::AGardenGridManager()
{
	PrimaryActorTick.bCanEverTick = false;

	GridDataComponent = CreateDefaultSubobject<UGridDataComponent>(TEXT("GridDataComponent"));
}

void AGardenGridManager::BeginPlay()
{
	Super::BeginPlay();

}
