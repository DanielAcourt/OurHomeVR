// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"
#include "BoidTypes.generated.h"

UENUM(BlueprintType)
enum class EBeeType : uint8
{
    Worker,
    Queen,
    Drone
};

USTRUCT(BlueprintType)
struct FBeeGeneticProfile
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Genetics")
    FGuid HomeHiveID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Genetics")
    FGuid LineageID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wisp|Genetics")
    EBeeType BeeType = EBeeType::Worker;

    UPROPERTY()
    FRandomStream GeneticSeed;

    FBeeGeneticProfile()
    {
        HomeHiveID.Invalidate();
        LineageID.Invalidate();
    }
};
