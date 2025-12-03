// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GardenGridManager.generated.h"

class UGridDataComponent;

UCLASS()
class WISPPLUGIN_API AGardenGridManager : public AActor
{
	GENERATED_BODY()

public:
	AGardenGridManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UGridDataComponent> GridDataComponent;
};
