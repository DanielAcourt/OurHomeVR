// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tenant.h"
#include "Persona.generated.h"

class UTenantRegistry;

/**
 * The core class for the LLM Governance framework, responsible for enforcing persona rules and tenants.
 */
UCLASS()
class LLMGOVERNANCEPLUGIN_API UPersona : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Initializes the Persona with a Tenant Registry.
     * @param InTenantRegistry The tenant registry to use.
     */
    void Initialize(UTenantRegistry* InTenantRegistry);

    /**
     * Adds a new tenant to the registry and logs the addition.
     * This is the primary entry point for adding a new tenant.
     * @param Tenant The tenant to add and log.
     */
    void AddNewTenantAndLog(const FTenant& Tenant);

private:
    /**
     * Logs the addition of a new tenant in a deterministic format.
     * Adheres to T001 (Deterministic Merge Engine Rules).
     * @param Tenant The tenant to log.
     */
    void logTenantAddition(const FTenant& Tenant);

    /** A reference to the tenant registry. */
    UPROPERTY()
    TObjectPtr<UTenantRegistry> TenantRegistry;
};
