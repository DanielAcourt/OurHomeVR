// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseClassCharacter.generated.h"

class USaveableEntityComponent;
class UQiComponent;

UCLASS()
class WISPPLUGIN_API ABaseClassCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseClassCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	virtual void BeginPlay() override;
};
