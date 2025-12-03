// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseClassActor.generated.h"

class USaveableEntityComponent;
class UQiComponent;

/**
 * @class ABaseClassActor
 * @brief A base class for actors in the game, providing common components.
 *
 * This class includes a SaveableEntityComponent and a QiComponent, which can be
 * used by derived classes.
 */
UCLASS()
class WISPPLUGIN_API ABaseClassActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	ABaseClassActor();

protected:
	/**
	 * @brief The component that manages the saveable state of this actor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	/**
	 * @brief The component that manages the Qi (life force) of this actor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
