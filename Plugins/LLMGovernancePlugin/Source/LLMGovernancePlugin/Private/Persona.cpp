// Copyright Epic Games, Inc. All Rights Reserved.

#include "Persona.h"
#include "TenantRegistry.h"

// Define a logging category for deterministic logging.
DEFINE_LOG_CATEGORY_STATIC(LogPersona, Log, All);

void UPersona::Initialize(UTenantRegistry* InTenantRegistry)
{
    TenantRegistry = InTenantRegistry;
}

void UPersona::AddNewTenantAndLog(const FTenant& Tenant)
{
    if (TenantRegistry)
    {
        TenantRegistry->AddTenant(Tenant);
        logTenantAddition(Tenant);
    }
    else
    {
        UE_LOG(LogPersona, Warning, TEXT("TenantRegistry is not initialized. Cannot add or log tenant."));
    }
}

void UPersona::logTenantAddition(const FTenant& Tenant)
{
    // Log the tenant information in a structured, deterministic format.
    // This adheres to T001 (Deterministic Merge Engine Rules).
    UE_LOG(LogPersona, Log, TEXT("[Tenant Added] ID: %s, Title: \"%s\", Description: \"%s\", Timestamp: %s"),
        *Tenant.ID,
        *Tenant.Title,
        *Tenant.Description,
        *Tenant.Timestamp.ToString());
}
