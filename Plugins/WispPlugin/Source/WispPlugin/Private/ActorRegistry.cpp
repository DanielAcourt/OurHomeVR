// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "ActorRegistry.h"
#include "GameFramework/Actor.h"
#include "SaveableEntityComponent.h"

void UActorRegistry::RegisterActor(AActor* Actor)
{
    if (!Actor) return;

    USaveableEntityComponent* SaveComponent = Actor->FindComponentByClass<USaveableEntityComponent>();
    if (SaveComponent && SaveComponent->InstanceID.IsValid())
    {
        RegistryMap.Add(SaveComponent->InstanceID, Actor);
    }
}

void UActorRegistry::UnregisterActor(AActor* Actor)
{
    if (!Actor) return;

    USaveableEntityComponent* SaveComponent = Actor->FindComponentByClass<USaveableEntityComponent>();
    if (SaveComponent && SaveComponent->InstanceID.IsValid())
    {
        RegistryMap.Remove(SaveComponent->InstanceID);
    }
}

AActor* UActorRegistry::FindActor(const FGuid& ActorID) const
{
    return RegistryMap.FindRef(ActorID);
}
