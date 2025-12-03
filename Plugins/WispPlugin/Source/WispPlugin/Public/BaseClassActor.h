// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseClassActor.generated.h"

class USaveableEntityComponent;
class UQiComponent;

UCLASS()
class WISPPLUGIN_API ABaseClassActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseClassActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	virtual void BeginPlay() override;
};
