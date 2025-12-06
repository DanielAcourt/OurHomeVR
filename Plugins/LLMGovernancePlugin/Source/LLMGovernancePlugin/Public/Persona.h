// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tenant.h"
#include "Persona.generated.h"

class UTenantRegistry;

/**
 * @class UPersona
 * @brief The core class for the LLM Governance framework.
 *
 * UPersona is responsible for enforcing persona rules and managing tenants. It acts as the central point of control
 * for the governance system, ensuring that all operations adhere to the established tenants. It is designed to be
 * extended with additional functionality for more complex governance scenarios.
 */
UCLASS()
class LLMGOVERNANCEPLUGIN_API UPersona : public UObject
{
    GENERATED_BODY()

public:
    /**
     * @brief Initializes the Persona with a Tenant Registry.
     *
     * This method must be called before any other operations are performed. It sets up the connection to the
     * tenant registry, which is used to store and retrieve tenants.
     *
     * @param InTenantRegistry A pointer to the UTenantRegistry instance to use.
     */
    void Initialize(UTenantRegistry* InTenantRegistry);

    /**
     * @brief Adds a new tenant to the registry and logs the addition.
     *
     * This is the primary entry point for adding a new tenant to the governance system. It ensures that the tenant
     * is correctly registered and that its addition is logged for auditing purposes.
     *
     * @param Tenant The FTenant instance to add and log.
     */
    void AddNewTenantAndLog(const FTenant& Tenant);

private:
    /**
     * @brief Logs the addition of a new tenant in a deterministic format.
     *
     * This method logs the tenant information in a structured and deterministic format, which is essential for
     * auditing and debugging. It adheres to T001 (Deterministic Merge Engine Rules).
     *
     * @param Tenant The FTenant instance to log.
     */
    void logTenantAddition(const FTenant& Tenant);

    /** @brief A reference to the tenant registry, responsible for storing and managing tenants. */
    UPROPERTY()
    TObjectPtr<UTenantRegistry> TenantRegistry;
};
