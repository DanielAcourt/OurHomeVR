// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoilDataComponent.h"

USoilDataComponent::USoilDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoilDataComponent::BeginPlay()
{
	Super::BeginPlay();
}
