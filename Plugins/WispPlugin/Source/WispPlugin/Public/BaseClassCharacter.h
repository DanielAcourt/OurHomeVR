// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseClassCharacter.generated.h"

class USaveableEntityComponent;
class UQiComponent;

/**
 * @class ABaseClassCharacter
 * @brief A base class for characters in the game, providing common components.
 *
 * This class includes a SaveableEntityComponent and a QiComponent, which can be
 * used by derived classes.
 */
UCLASS()
class WISPPLUGIN_API ABaseClassCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	ABaseClassCharacter();

protected:
	/**
	 * @brief The component that manages the saveable state of this character.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USaveableEntityComponent> SaveableEntityComponent;

	/**
	 * @brief The component that manages the Qi (life force) of this character.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQiComponent> QiComponent;

	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
