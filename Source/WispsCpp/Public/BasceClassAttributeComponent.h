// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h" // Our structs
#include "BasceClassAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WISPSCPP_API UBasceClassAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasceClassAttributeComponent();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCompnent")
	FQiData QiData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeCompnent")
	FAtrributesData AttributeData;

};
