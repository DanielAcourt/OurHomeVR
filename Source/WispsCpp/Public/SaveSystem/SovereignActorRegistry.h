// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SovereignActorRegistry.generated.h"

class AActor;

/**
 * @class USovereignActorRegistry
 * @brief The "Sovereign Live Registry" or "King's Ledger."
 * Acting as a World Subsystem, this is the central hub for tracking every "Saveable" entity
 * currently active. This avoids expensive 'GetAllActorsOfClass' calls during
 * save/load cycles, ensuring high performance.
 */
UCLASS()
class WISPCPP_API USovereignActorRegistry : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    /**
     * Adds an actor to the live tracking system.
     * @param EntityID The unique Passport ID (GUID) for the actor.
     * @param Actor The actor to register. Called by a saveable entity in BeginPlay.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Registry")
    void RegisterActor(FGuid EntityID, AActor* Actor);

    /**
     * Removes an actor from tracking.
     * @param EntityID The unique Passport ID of the actor to unregister. Called by a saveable entity in EndPlay.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Registry")
    void UnregisterActor(FGuid EntityID);

    /**
     * Finds a specific actor using its unique Passport ID (GUID).
     * @param EntityID The ID of the actor to find.
     * @return The found actor, or nullptr if not found or invalid.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Registry")
    AActor* FindActor(FGuid EntityID) const;

    /**
     * Gathers a list of all currently valid, tracked actors.
     * @return An array of valid actor pointers.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Registry")
    TArray<AActor*> GetTrackedActorsAsList() const;

    /**
     * Returns the complete, underlying registry map. Useful for the SaveManager loop.
     * @return A const reference to the TMap of IDs to weak actor pointers.
     */
    const TMap<FGuid, TWeakObjectPtr<AActor>>& GetActiveRegistry() const { return TrackedActors; }

private:
    /**
     * The internal storage, mapping a Unique ID to a physical Actor in the world.
     * Using TWeakObjectPtr prevents crashes if an Actor is destroyed
     * without UnregisterActor being called, which is a critical safety feature.
     */
    UPROPERTY()
    TMap<FGuid, TWeakObjectPtr<AActor>> TrackedActors;
};
