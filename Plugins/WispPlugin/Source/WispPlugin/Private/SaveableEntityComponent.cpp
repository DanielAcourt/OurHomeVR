// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveableEntityComponent.h"

USaveableEntityComponent::USaveableEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveableEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// If our Instance ID is not valid (i.e., this is a new object), create a new one.
	if (!InstanceID.IsValid())
	{
		InstanceID = FGuid::NewGuid();
	}
}
