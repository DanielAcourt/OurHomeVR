#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AWispPlayerController.generated.h"

class UInteractionComponent;
class UUserWidget;

/**
 * @class AWispPlayerController
 * @brief Manages the player's interactions with the game world.
 *
 * This class is responsible for handling player input, managing interactions with
 * interactable objects, and displaying relevant UI widgets.
 */
UCLASS()
class WISPPLUGIN_API AWispPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	AWispPlayerController();

	/**
	 * @brief Called by an interactable object when the player chooses to manage it.
	 * @param EntityToManage The actor that the player will start managing.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void BeginManagingEntity(AActor* EntityToManage);

	/**
	 * @brief Called by the UI when the player is finished managing an object.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndManagingEntity();

protected:
	/**
	 * @brief The component that handles looking for interactable objects.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	/**
	 * @brief An array of all entities we are currently managing. Index 0 is the primary.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<TObjectPtr<AActor>> ManagedEntities;

	/**
	 * @brief The currently open UI widget for the managed entity.
	 */
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentInteractionWidget;

	/**
	 * @brief Sets up the input component for the player controller.
	 */
	virtual void SetupInputComponent() override;

private:
	/**
	 * @brief Called when the "Interact" input action is pressed.
	 */
	void OnInteractPressed();
};
