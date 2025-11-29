// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "PlantDataComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UPlantDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlantDataComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FPlantData PlantData;

protected:
	virtual void BeginPlay() override;
};
