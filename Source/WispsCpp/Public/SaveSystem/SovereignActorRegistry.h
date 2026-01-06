// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SovereignActorRegistry.generated.h"

class AActor;

/**
 * @class USovereignActorRegistry
 * @brief The King's Ledger. A centralized registry for all sovereign entities in the world.
 * This subsystem provides an O(1) lookup for actors using their FGuid-based EntityID.
 * It is the single source of truth for which actors are currently active and saveable.
 */
UCLASS()
class WISPCPP_API USovereignActorRegistry : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Registers an actor with the subsystem.
	 * @param Actor The actor to register. Must not be null.
	 * @param EntityID The unique identifier for the actor.
	 */
	void RegisterActor(AActor* Actor, const FGuid& EntityID);

	/**
	 * Unregisters an actor from the subsystem.
	 * @param EntityID The unique identifier of the actor to unregister.
	 */
	void UnregisterActor(const FGuid& EntityID);

	/**
	 * Finds an actor by its unique identifier.
	 * @param EntityID The unique identifier of the actor to find.
	 * @return A pointer to the actor if found, otherwise nullptr.
	 */
	AActor* FindActor(const FGuid& EntityID) const;

    /**
     * Gets the entire actor registry map.
     * @return A const reference to the registry map.
     */
    const TMap<FGuid, AActor*>& GetRegistry() const;

private:
	/** The main registry mapping EntityIDs to Actor pointers. */
	UPROPERTY()
	TMap<FGuid, AActor*> ActorRegistry;
};
