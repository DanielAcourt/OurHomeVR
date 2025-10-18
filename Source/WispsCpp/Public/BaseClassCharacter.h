// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JsonUtility.h"
#include "BaseClassCharacter.generated.h"

// Forward declare our component classes
class USaveableEntityComponent;
class UQiComponent;


UCLASS()
class WISPSCPP_API ABaseClassCharacter : public ACharacter
{

	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties


	//Jules could we put variable here so that when the character spawn it has core variable for in game here. Or is it best to do everything in blueprints?
	ABaseClassCharacter();


protected:

	// -- CORE COMPONENTS --
	// Add this to make every child of this class saveable by default.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	// Add this to give every child Qi by default.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	virtual void BeginPlay() override;

	


public:	

	/*
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	*/

private:



};
