// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonUtility.h"
#include "BaseClassActor.generated.h"

// Forward declare our component classes
class USaveableEntityComponent;
class UQiComponent;


// Do i want to include attribute on all actors like dnd logic?
//class UAttributes;

//But soil doesnt have attributes like strength and health Just fetility and the chance to store Qi
//This makes have the basics but also the ability to include your own  bespoke commonents relevant to your game design.


UCLASS()
class WISPPLUGIN_API ABaseClassActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseClassActor();

protected:
	// -- CORE COMPONENTS --
	
// Add this to make every child of this class saveable by default.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	// Add this to give every child Qi by default if you want every object to have magic.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	virtual void BeginPlay() override;
	

public:	

};
