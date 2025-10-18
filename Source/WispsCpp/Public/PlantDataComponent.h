// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h" // Make sure this is included
#include "PlantDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WISPSCPP_API UPlantDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlantDataComponent();


public:	

	// Declar the plant has the component Plant Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FPlantData PlantData;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
