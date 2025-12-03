// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h"
#include "SoilDataComponent.generated.h"

/**
 * @class USoilDataComponent
 * @brief An actor component that holds data for a patch of soil.
 *
 * This component stores an FSoilData struct, which contains information about
 * the soil's fertility, water content, and Qi data.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API USoilDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	USoilDataComponent();

	/**
	 * @brief The data for the soil.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil Data")
	FSoilData SoilData;

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
};
