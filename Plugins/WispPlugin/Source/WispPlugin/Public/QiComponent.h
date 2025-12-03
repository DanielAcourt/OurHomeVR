// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "QiComponent.generated.h"

/**
 * @class UQiComponent
 * @brief An actor component that holds data related to Qi (life force).
 *
 * This component stores an FQiData struct, which contains information about
 * an actor's current Qi, max Qi, and Qi generation rate.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UQiComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UQiComponent();

	/**
	 * @brief The Qi data for this component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	FQiData QiData;

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
