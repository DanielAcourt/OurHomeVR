// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SovereignSaveManager.generated.h"

class USovereignSaveGame;

/**
 * @class USovereignSaveManager
 * @brief The "Brain" of the Sovereign Save System.
 * As a GameInstance Subsystem, it persists across all level changes, acting as the central
 * authority for all save and load operations.
 */
UCLASS()
class WISPCPP_API USovereignSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    // UGameInstanceSubsystem overrides
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /**
     * Gathers the state of all registered, saveable entities and commits them to a save slot.
     * @param SlotName The file name for the save.
     * @param bAsJson If true, writes a human-readable .json file. If false, writes a binary .sav file.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|SaveSystem", meta = (DisplayName = "Save World State"))
    void SaveWorldState(const FString& SlotName, bool bAsJson = true);

    /**
     * Loads the state from a save slot, destroying/spawning/updating entities to match the saved data.
     * @param SlotName The file name of the save to load.
     * @param bAsJson If true, looks for a .json file. If false, looks for a binary .sav file.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|SaveSystem", meta = (DisplayName = "Load World State"))
    void LoadWorldState(const FString& SlotName, bool bAsJson = true);

    /**
     * Peeks into a save slot and returns the number of entities stored within, without altering the world state.
     * @param SlotName The file name of the save to inspect.
     * @param bAsJson If true, looks for a .json file. If false, looks for a binary .sav file.
     * @return The number of entities in the save file, or 0 if the file doesn't exist or is empty.
     */
    UFUNCTION(BlueprintPure, Category = "Sovereign|SaveSystem")
    int32 GetSavedEntityCount(const FString& SlotName, bool bAsJson = true);

private:
    /** Destroys any registered actor in the world that is not found in the provided save data. */
    void PurgeUnearnedEntities(USovereignSaveGame* LoadedSuitcase);

    /** Converts a USovereignSaveGame object into a JSON string. */
    FString ConvertSuitcaseToJson(USovereignSaveGame* Suitcase);

    /** Creates a USovereignSaveGame object from a JSON string. */
    USovereignSaveGame* ConvertJsonToSuitcase(const FString& JsonContent);

    /** Gets the full, sanitized file path for a given save slot. */
	FString GetSaveFilePath(const FString& SlotName) const;
};
