// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#include "SaveSystem/SovereignActorRegistry.h"
#include "GameFramework/Actor.h"

void USovereignActorRegistry::RegisterActor(FGuid EntityID, AActor* Actor)
{
    if (EntityID.IsValid() && Actor)
    {
        // Safety Check: Ensure this ID isn't already registered to a DIFFERENT valid actor.
        if (TrackedActors.Contains(EntityID) && TrackedActors[EntityID].IsValid() && TrackedActors[EntityID] != Actor)
        {
            UE_LOG(LogTemp, Error, TEXT("SovereignActorRegistry: ID COLLISION! Actor '%s' tried to take ID [%s] already held by '%s'."),
                *Actor->GetName(), *EntityID.ToString(), *TrackedActors[EntityID]->GetName());
            return;
        }

        TrackedActors.Add(EntityID, Actor);
        UE_LOG(LogTemp, Log, TEXT("SovereignActorRegistry: Registered %s with ID [%s]"), *Actor->GetName(), *EntityID.ToString());
    }
}

void USovereignActorRegistry::UnregisterActor(FGuid EntityID)
{
    if (EntityID.IsValid() && TrackedActors.Contains(EntityID))
    {
        // Log the removal for debugging save cycles.
        const FString ActorName = TrackedActors[EntityID].IsValid() ? TrackedActors[EntityID]->GetName() : TEXT("Invalid/Destroyed Actor");
        TrackedActors.Remove(EntityID);

        UE_LOG(LogTemp, Warning, TEXT("SovereignActorRegistry: Unregistered Actor '%s'. ID [%s] has been vacated."), *ActorName, *EntityID.ToString());
    }
}

AActor* USovereignActorRegistry::FindActor(FGuid EntityID) const
{
    const TWeakObjectPtr<AActor>* FoundPtr = TrackedActors.Find(EntityID);

    if (FoundPtr && FoundPtr->IsValid())
    {
        return FoundPtr->Get();
    }
    return nullptr;
}

TArray<AActor*> USovereignActorRegistry::GetTrackedActorsAsList() const
{
    TArray<AActor*> OutList;
    OutList.Reserve(TrackedActors.Num());

    for (const auto& Pair : TrackedActors)
    {
        if (Pair.Value.IsValid())
        {
            OutList.Add(Pair.Value.Get());
        }
    }
    return OutList;
}
