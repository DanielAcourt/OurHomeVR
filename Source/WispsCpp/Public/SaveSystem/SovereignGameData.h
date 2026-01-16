// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SovereignGameData.generated.h"

class USovereignSpeciesData;

/**
 * @struct FItemDefaults
 * @brief Defines a row in a data table for default species information.
 * Links a Gameplay Tag to a data asset.
 */
USTRUCT(BlueprintType)
struct FItemDefaults : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sovereign|Identity")
    FGameplayTag SpeciesTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sovereign|Data")
    TSoftObjectPtr<USovereignSpeciesData> SpeciesVesselData;
};

/**
 * @enum EGrowthState
 * @brief Represents the different growth stages of an entity.
 */
UENUM(BlueprintType)
enum class EGrowthState : uint8
{
    Seed,
    Sprout,
    Mature
};

/**
 * @enum EUpdateFrequency
 * @brief Defines how often an entity's logic should be updated by a manager subsystem.
 */
UENUM(BlueprintType)
enum class EUpdateFrequency : uint8
{
    Realtime    UMETA(DisplayName = "Every Frame (Wisps/Animals)"),
    Standard    UMETA(DisplayName = "Every 1 Second (Plants/Growth)"),
    Slow        UMETA(DisplayName = "Every 10 Seconds (Geology/Rocks)"),
    Dormant     UMETA(DisplayName = "Only on Interaction (Statues/Fences)")
};

/**
 * @enum ESovereignElement
 * @brief Represents the core elemental alignments in the game.
 */
UENUM(BlueprintType)
enum class ESovereignElement : uint8
{
    None,
    // Alignment
    Light, Dark, Grey,
    // Core Natural
    Fire, Water, Earth, Nature, Air,
    // Special
    Electric, Fairy, Dragon
};

/**
 * @struct FEntitySaveData
 * @brief The core data structure for a single saveable entity. This is the blueprint for the JSON.
 */
USTRUCT(BlueprintType)
struct FEntitySaveData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Identity")
    FGuid MyGUID;

    /** The blueprint path of the actor, essential for respawning. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Identity")
    FString ClassPath;

    /** The physical state of the actor in the world. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Physicality")
    FTransform WorldTransform;

    /** The GUID of the entity that created this object. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    FGuid ParentID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    FGuid MotherID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    FGuid FatherID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    bool bIsFemale;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    int32 OffspringCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    TArray<FGuid> MatingHistory;

    /** The growth state of the entity. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|State")
    EGrowthState CurrentState;

    /** The saved update frequency for manager subsystems. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|State")
    EUpdateFrequency SavedFrequency;

    /**
     * A flexible map to handle tags the system can't know in advance.
     * This is where component data (Qi, Attributes, etc.) gets serialized.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|State")
    TMap<FString, FString> UnknownMetaTags;
};

/**
 * @struct FPSTAS_Scores
 * @brief Holds the four core performance and trust scores for a Sovereign entity.
 */
USTRUCT(BlueprintType)
struct FPSTAS_Scores
{
    GENERATED_BODY()

    /** P (Psychological/Focus): Entity's adherence to its current task/goal. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PSTAS")
    float P_Score = 100.0f;

    /** S (Social/Alignment): Entity's adherence to its defined alignment and faction rules. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PSTAS")
    float S_Score = 100.0f;

    /** T (Technical/Environmental): Entity's successful interaction with the world (e.g., pathfinding, resource gathering). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PSTAS")
    float T_Score = 100.0f;

    /** A (Administrative/Compliance): Entity's adherence to the framework's rules (e.g., registered, has valid ID). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PSTAS")
    float A_Score = 100.0f;
};

/**
 * @enum EVerifiedSafetyStatus
 * @brief Represents the final, mapped status of an entity's VSS score.
 */
UENUM(BlueprintType)
enum class EVerifiedSafetyStatus : uint8
{
    /** VSS <= 0: The entity has critically failed in at least one dimension. Trust is zero. */
    Critical UMETA(DisplayName = "Critical"),

    /** 0 < VSS <= 35: The entity is in a dangerous state and likely to fail. */
    Warning  UMETA(DisplayName = "Warning"),

    /** 35 < VSS <= 70: The entity is underperforming and needs monitoring. */
    Caution  UMETA(DisplayName = "Caution"),

    /** VSS > 70: The entity is performing as expected. */
    Nominal  UMETA(DisplayName = "Nominal")
};
