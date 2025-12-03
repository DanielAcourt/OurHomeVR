// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "GardenData.generated.h"

/**
 * @enum EGrowthState
 * @brief Represents the different growth states of a plant.
 */
UENUM(BlueprintType)
enum class EGrowthState : uint8
{
	GS_None			UMETA(DisplayName = "None"),
	GS_Sprout		UMETA(DisplayName = "Sprout"),
	GS_Seedling		UMETA(DisplayName = "Seedling"),
	GS_Mature		UMETA(DisplayName = "Mature"),
	GS_Fruiting		UMETA(DisplayName = "Fruiting"),
	GS_Wilted		UMETA(DisplayName = "Wilted"),
};

/**
 * @enum EUpdateFrequency
 * @brief Defines how often a component should be updated.
 */
UENUM(BlueprintType)
enum class EUpdateFrequency : uint8
{
	UF_High			UMETA(DisplayName = "High"),		// Every 1-5 seconds
	UF_Medium		UMETA(DisplayName = "Medium"),	// Every 15-30 seconds
	UF_Low			UMETA(DisplayName = "Low"),		// Every 60+ seconds
	UF_Dormant		UMETA(DisplayName = "Dormant"),	// Only on load/special event
};

/**
 * @enum ETrainingFocus
 * @brief Defines the training focus for a plant.
 */
UENUM(BlueprintType)
enum class ETrainingFocus : uint8
{
	TF_None			UMETA(DisplayName = "None"),
	TF_Roots		UMETA(DisplayName = "Roots"),		// Growth, Resilience
	TF_Leaves		UMETA(DisplayName = "Leaves"),		// Qi Production, Pest Resistance
	TF_Flowers		UMETA(DisplayName = "Flowers"),	// Attractiveness, Fruit Quality
};

/**
 * @struct FQiData
 * @brief A struct to hold data related to Qi (life force).
 */
USTRUCT(BlueprintType)
struct FQiData
{
	GENERATED_BODY()

	/** The current amount of Qi. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float CurrentQi = 0.0f;

	/** The maximum amount of Qi. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float MaxQi = 100.0f;

	/** The rate at which Qi is generated. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float QiGenerationRate = 1.0f;
};

/**
 * @struct FAttributesData
 * @brief A struct to hold data related to a plant's attributes.
 */
USTRUCT(BlueprintType)
struct FAttributesData
{
	GENERATED_BODY()

	/** The growth attribute of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Growth = 0;

	/** The resilience attribute of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Resilience = 0;

	/** The pest resistance attribute of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 PestResistance = 0;

	/** The attractiveness attribute of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Attractiveness = 0;

	/** The fruit quality attribute of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 FruitQuality = 0;
};

/**
 * @struct FSoilData
 * @brief A struct to hold data related to soil.
 */
USTRUCT(BlueprintType)
struct FSoilData
{
	GENERATED_BODY()

	/** The fertility of the soil. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	float Fertility = 1.0f;

	/** The water content of the soil. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	float WaterContent = 1.0f;

	/** The Qi data of the soil. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	FQiData QiData;
};

/**
 * @struct FPlantData
 * @brief A struct to hold data related to a plant.
 */
USTRUCT(BlueprintType)
struct FPlantData : public FTableRowBase
{
	GENERATED_BODY()

	/** The gameplay tag of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FGameplayTag PlantTag;

	/** The current growth state of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	EGrowthState GrowthState = EGrowthState::GS_None;

	/** The attributes of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FAttributesData Attributes;

	/** The Qi data of the plant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FQiData QiData;
};
