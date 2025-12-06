// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TenantRegistry.generated.h"

struct FTenant;

/**
 * @class UTenantRegistry
 * @brief A registry to handle the storage, lookup, and validation of tenants.
 *
 * The UTenantRegistry is responsible for managing the lifecycle of tenants within the governance framework. It
 * provides a centralized location for storing, retrieving, and validating tenants, ensuring that the system has a
 * consistent and up-to-date view of all governance rules.
 */
UCLASS()
class LLMGOVERNANCEPLUGIN_API UTenantRegistry : public UObject
{
    GENERATED_BODY()

public:
    /**
     * @brief Adds a new tenant to the registry.
     *
     * This method adds a new tenant to the internal storage, making it available for lookup and validation.
     *
     * @param Tenant The FTenant instance to add.
     */
    void AddTenant(const FTenant& Tenant);

    /**
     * @brief Retrieves a tenant from the registry by its ID.
     *
     * This method allows for the retrieval of a specific tenant by its unique identifier.
     *
     * @param TenantID The ID of the tenant to retrieve.
     * @return A const pointer to the FTenant instance if found, otherwise nullptr.
     */
    const FTenant* GetTenant(const FString& TenantID) const;

private:
    /** @brief TMap to store tenants, indexed by their ID for efficient lookup. */
    UPROPERTY()
    TMap<FString, FTenant> TenantMap;
};
