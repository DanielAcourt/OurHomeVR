// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlantDataComponent.h"

UPlantDataComponent::UPlantDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlantDataComponent::BeginPlay()
{
	Super::BeginPlay();
}
