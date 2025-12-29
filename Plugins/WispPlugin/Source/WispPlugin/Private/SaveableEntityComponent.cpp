// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveableEntityComponent.h"
#include "ActorRegistry.h"
#include "Engine/World.h"

USaveableEntityComponent::USaveableEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveableEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InstanceID.IsValid())
	{
		InstanceID = FGuid::NewGuid();
	}

    if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
    {
        UActorRegistry* Registry = GameInstance->GetSubsystem<UActorRegistry>();
        if (Registry)
        {
            Registry->RegisterActor(GetOwner());
        }
    }
}

void USaveableEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (UGameInstance* GameInstance = GetOwner()->GetGameInstance())
    {
        UActorRegistry* Registry = GameInstance->GetSubsystem<UActorRegistry>();
        if (Registry)
        {
            Registry->UnregisterActor(GetOwner());
        }
    }
}
