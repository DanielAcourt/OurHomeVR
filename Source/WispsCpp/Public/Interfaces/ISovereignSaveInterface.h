// Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SovereignSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USovereignSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ISovereignSaveInterface
 * @brief The Law. This interface defines the contract for any object that wishes to be part of the Sovereign Save System.
 * It provides a standardized way to capture and restore an object's state.
 */
class WISPCPP_API ISovereignSaveInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called when the Save Manager requests the object's data to be saved.
	 * @return A TMap containing the object's state to be serialized.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Sovereign Save System")
	TMap<FName, FString> GetSaveData() const;

	/**
	 * Called when the Save Manager is loading data and needs this object to restore its state.
	 * @param InData The TMap containing the state to restore.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Sovereign Save System")
	void LoadSaveData(const TMap<FName, FString>& InData);
};
