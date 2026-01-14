// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveSystem/SovereignGameData.h"
#include "Interfaces/SovereignSaveInterface.h"
#include "SovereignSaveableEntityComponent.generated.h"

class ASovereignBaseEntity;

/**
 * @class USovereignSaveableEntityComponent
 * @brief The "Passport." This component is the heart of the Sovereign save system.
 * It lives on an actor, assigns it a unique identity, and is responsible for capturing
 * all of its relevant state from itself and other components.
 */
UCLASS(ClassGroup = (Sovereign), meta = (BlueprintSpawnableComponent))
class WISPCPP_API USovereignSaveableEntityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USovereignSaveableEntityComponent();

    /**
     * The master "scrape" function. Aggregates data from this component, actor-level tags,
     * and any other component that implements the ISovereignSaveInterface.
     * @return A map of all data to be saved for this entity.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Persistence")
    TMap<FString, FString> CaptureFullEntityState();

    /** Applies loaded data back to this component and the actor's tags. */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|Persistence")
    void ApplyMetaTags(const TMap<FString, FString>& LoadedTags);

    /** --- 1. IDENTITY --- */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Identity")
    FGuid EntityID;

    /** --- 2. LINEAGE & BREEDING --- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    bool bIsFemale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    FGuid ParentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    FGuid MotherID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    FGuid FatherID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    int32 OffspringCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    TArray<FGuid> MatingHistory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|Lineage")
    float LastMatingTimestamp = -100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Lineage")
    float MatingCooldownDuration = 60.0f;

    /** --- 3. THE TRIPLE-AXIS SYSTEM (The Soul, Body, and Spark) --- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics")
    ESovereignElement AlignmentSocket = ESovereignElement::Grey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics")
    ESovereignElement BodySocket = ESovereignElement::Nature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics")
    ESovereignElement MagicSocket = ESovereignElement::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics", meta = (ClampMin = "0", ClampMax = "100"))
    float AlignmentInfluence = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics", meta = (ClampMin = "0", ClampMax = "100"))
    float BodyInfluence = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Genetics", meta = (ClampMin = "0", ClampMax = "100"))
    float MagicInfluence = 0.0f;

    /** --- 4. EVOLUTION & ENERGY --- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Evolution")
    float MaturityProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sovereign|Evolution")
    float MaturityRate = 0.1f;

    UFUNCTION(BlueprintCallable, Category = "Sovereign|Evolution")
    void ReceiveElementalEnergy(ESovereignElement EnergyType, float RawAmount);

    /** --- 5. PERFORMANCE & TRUST --- */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|PSTAS")
    FPSTAS_Scores PSTAS;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign|PSTAS")
    EVerifiedSafetyStatus CurrentVSS;

    /**
     * Recalculates all PSTAS scores and the final VSS based on the entity's current state.
     * This is an event-driven function and should not be called on Tick.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign|PSTAS")
    void RefreshVerifiedStatus();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /** Calculates multipliers based on the 3-axis interaction. */
    float GetElementalMultiplier(ESovereignElement IncomingType);

    /** Scrapes simple "Key:Value" tags from the owning actor. */
    TMap<FString, FString> GetActorMetaTags() const;

#if WITH_EDITOR
    // Ensures a new GUID is generated when the component is added in the editor or duplicated.
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostDuplicate(bool bDuplicateForPIE) override;
#endif
};
