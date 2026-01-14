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
 * @brief The "Save Contract." Any UActorComponent that implements this interface will have its
 * data automatically collected and saved by the USovereignSaveableEntityComponent.
 */
class WISPCPP_API ISovereignSaveInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called by the Saveable Entity Component to request the component's data for serialization.
	 * @return A TMap containing the component's state to be saved. Keys should be unique to this component.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Sovereign Save System")
	TMap<FName, FString> GetSaveData() const;

	/**
	 * Called by the Saveable Entity Component when loading data, providing the saved state.
	 * @param InData The TMap containing the state to restore.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Sovereign Save System")
	void LoadSaveData(const TMap<FName, FString>& InData);
};
