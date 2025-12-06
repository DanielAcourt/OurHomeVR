// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Tenant.generated.h"

/**
 * @struct FTenant
 * @brief Represents a single tenant, a rule or principle for LLM-assisted development.
 *
 * A tenant is a fundamental component of the LLM Governance framework. It defines a specific rule, guideline, or
 * principle that must be adhered to during the development process. Each tenant is uniquely identifiable and
 * contains metadata to describe its purpose and history.
 */
USTRUCT(BlueprintType)
struct FTenant
{
    GENERATED_BODY()

    /** @brief The unique identifier for the tenant, e.g., "T001". */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FString ID;

    /** @brief The title of the tenant, providing a concise summary of the rule. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FString Title;

    /** @brief A detailed description of the tenant, explaining the rule in more detail. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FString Description;

    /** @brief The timestamp when the tenant was added or last modified. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tenant")
    FDateTime Timestamp;
};
