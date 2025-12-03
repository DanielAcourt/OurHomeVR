// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "GardenData.generated.h"

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

UENUM(BlueprintType)
enum class EUpdateFrequency : uint8
{
	UF_High			UMETA(DisplayName = "High"),		// Every 1-5 seconds
	UF_Medium		UMETA(DisplayName = "Medium"),	// Every 15-30 seconds
	UF_Low			UMETA(DisplayName = "Low"),		// Every 60+ seconds
	UF_Dormant		UMETA(DisplayName = "Dormant"),	// Only on load/special event
};

UENUM(BlueprintType)
enum class ETrainingFocus : uint8
{
	TF_None			UMETA(DisplayName = "None"),
	TF_Roots		UMETA(DisplayName = "Roots"),		// Growth, Resilience
	TF_Leaves		UMETA(DisplayName = "Leaves"),		// Qi Production, Pest Resistance
	TF_Flowers		UMETA(DisplayName = "Flowers"),	// Attractiveness, Fruit Quality
};

USTRUCT(BlueprintType)
struct FQiData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float CurrentQi = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float MaxQi = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	float QiGenerationRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FAttributesData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Growth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Resilience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 PestResistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 Attractiveness = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 FruitQuality = 0;
};

USTRUCT(BlueprintType)
struct FSoilData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	float Fertility = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	float WaterContent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil")
	FQiData QiData;
};

USTRUCT(BlueprintType)
struct FPlantData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FGameplayTag PlantTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	EGrowthState GrowthState = EGrowthState::GS_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FAttributesData Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	FQiData QiData;
};
