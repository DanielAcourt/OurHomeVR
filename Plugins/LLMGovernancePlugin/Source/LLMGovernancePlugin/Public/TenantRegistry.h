// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TenantRegistry.generated.h"

struct FTenant;

/**
 * A registry to handle the storage, lookup, and validation of tenants.
 */
UCLASS()
class LLMGOVERNANCEPLUGIN_API UTenantRegistry : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Adds a new tenant to the registry.
     * @param Tenant The tenant to add.
     */
    void AddTenant(const FTenant& Tenant);

    /**
     * Retrieves a tenant from the registry by its ID.
     * @param TenantID The ID of the tenant to retrieve.
     * @return A const pointer to the tenant if found, otherwise nullptr.
     */
    const FTenant* GetTenant(const FString& TenantID) const;

private:
    // TMap to store tenants, indexed by their ID.
    UPROPERTY()
    TMap<FString, FTenant> TenantMap;
};
