// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "SaveSystem/SovereignActorRegistry.h"
#include "GameFramework/Actor.h"

void USovereignActorRegistry::RegisterActor(AActor* Actor, const FGuid& EntityID)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("USovereignActorRegistry::RegisterActor: Attempted to register a null actor."));
		return;
	}

	if (!EntityID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("USovereignActorRegistry::RegisterActor: Attempted to register actor '%s' with an invalid EntityID."), *Actor->GetName());
		return;
	}

	if (ActorRegistry.Contains(EntityID))
	{
		UE_LOG(LogTemp, Warning, TEXT("USovereignActorRegistry::RegisterActor: EntityID '%s' is already registered. Overwriting actor '%s' with '%s'."), *EntityID.ToString(), *ActorRegistry[EntityID]->GetName(), *Actor->GetName());
	}

	ActorRegistry.Add(EntityID, Actor);
}

void USovereignActorRegistry::UnregisterActor(const FGuid& EntityID)
{
	if (!EntityID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("USovereignActorRegistry::UnregisterActor: Attempted to unregister an invalid EntityID."));
		return;
	}

	if (!ActorRegistry.Contains(EntityID))
	{
		UE_LOG(LogTemp, Warning, TEXT("USovereignActorRegistry::UnregisterActor: EntityID '%s' not found in registry."), *EntityID.ToString());
		return;
	}

	ActorRegistry.Remove(EntityID);
}

AActor* USovereignActorRegistry::FindActor(const FGuid& EntityID) const
{
	if (!EntityID.IsValid())
	{
		return nullptr;
	}

	return ActorRegistry.FindRef(EntityID);
}

const TMap<FGuid, AActor*>& USovereignActorRegistry::GetRegistry() const
{
    return ActorRegistry;
}
