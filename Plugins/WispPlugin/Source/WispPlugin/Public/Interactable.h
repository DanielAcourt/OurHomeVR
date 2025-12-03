#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IInteractable
 * @brief An interface for actors that can be interacted with by the player.
 *
 * This interface provides functions that are called when the player hovers over,
 * stops hovering over, or interacts with an actor.
 */
class WISPPLUGIN_API IInteractable
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when the player begins to hover over the actor.
	 *
	 * C++ classes can implement this function to be called when hovered.
	 * Blueprints will implement the event "Event On Begin Hover".
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnBeginHover();

	/**
	 * @brief Called when the player stops hovering over the actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnEndHover();

	/**
	 * @brief Called when the player interacts with the actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract();
};
