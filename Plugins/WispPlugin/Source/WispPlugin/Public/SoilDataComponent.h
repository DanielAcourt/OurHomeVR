// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "SoilDataComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API USoilDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoilDataComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil Data")
	FSoilData SoilData;

protected:
	virtual void BeginPlay() override;
};
