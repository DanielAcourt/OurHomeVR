// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Tenant.generated.h"

/**
 * Represents a single tenant, a rule or principle for LLM-assisted development.
 */
USTRUCT(BlueprintType)
struct FTenant
{
    GENERATED_BODY()

    /** The unique identifier for the tenant, e.g., "T001". */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FString ID;

    /** The title of the tenant. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FString Title;

    /** The timestamp when the tenant was added or last modified. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FDateTime Timestamp;
};
