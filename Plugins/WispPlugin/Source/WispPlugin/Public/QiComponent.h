// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "QiComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UQiComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQiComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	FQiData QiData;

protected:
	virtual void BeginPlay() override;
};
