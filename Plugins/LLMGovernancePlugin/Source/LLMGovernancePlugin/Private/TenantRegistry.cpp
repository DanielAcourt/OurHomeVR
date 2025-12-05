// Copyright Epic Games, Inc. All Rights Reserved.

#include "TenantRegistry.h"
#include "Tenant.h"

void UTenantRegistry::AddTenant(const FTenant& Tenant)
{
    if (!Tenant.ID.IsEmpty())
    {
        TenantMap.Add(Tenant.ID, Tenant);
    }
}

const FTenant* UTenantRegistry::GetTenant(const FString& TenantID) const
{
    return TenantMap.Find(TenantID);
}
