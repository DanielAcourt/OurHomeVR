#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AWispPlayerController.generated.h"

class UInteractionComponent;
class UUserWidget;

UCLASS()
class WISPPLUGIN_API AWispPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWispPlayerController();

	// Called by an interactable object when the player chooses to manage it.
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void BeginManagingEntity(AActor* EntityToManage);

	// Called by the UI when the player is finished managing an object.
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndManagingEntity();

protected:
	// The component that handles looking for interactable objects.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	// An array of all entities we are currently managing. Index 0 is the primary.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<TObjectPtr<AActor>> ManagedEntities;

	// The currently open UI widget for the managed entity.
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentInteractionWidget;

	virtual void SetupInputComponent() override;

private:
	// Called when the "Interact" input action is pressed.
	void OnInteractPressed();
};