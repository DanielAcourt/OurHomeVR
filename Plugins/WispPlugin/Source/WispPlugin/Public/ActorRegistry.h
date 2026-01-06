// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorRegistry.generated.h"

UCLASS()
class WISPPLUGIN_API UActorRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    void RegisterActor(AActor* Actor);
    void UnregisterActor(AActor* Actor);
    AActor* FindActor(const FGuid& ActorID) const;
    const TMap<FGuid, AActor*>& GetRegistryMap() const { return RegistryMap; };

private:
    TMap<FGuid, AActor*> RegistryMap;
};
