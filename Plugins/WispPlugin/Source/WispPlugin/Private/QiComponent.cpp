// Copyright Epic Games, Inc. All Rights Reserved.

#include "QiComponent.h"

UQiComponent::UQiComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQiComponent::BeginPlay()
{
	Super::BeginPlay();
}
