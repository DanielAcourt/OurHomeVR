#include "AWispPlayerController.h"
#include "InteractionComponent.h"
#include "SaveableEntityComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"



AWispPlayerController::AWispPlayerController()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void AWispPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// This assumes you have an Input Action named "Interact" in your project settings.
	InputComponent->BindAction("Interact", IE_Pressed, this, &AWispPlayerController::OnInteractPressed);
}

void AWispPlayerController::OnInteractPressed()
{
	// Tell the interaction component to fire its "Interact" event.
	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}

void AWispPlayerController::BeginManagingEntity(AActor* EntityToManage)
{
	if (!EntityToManage) return;

	ManagedEntities.Add(EntityToManage);

	// Disable movement on the pawn so the player can't fly away.
	if (GetPawn())
	{
		GetPawn()->DisableInput(this);
	}

	
	// Get the widget class from the entity and create it.
	if (USaveableEntityComponent* SaveableComponent = EntityToManage->FindComponentByClass<USaveableEntityComponent>())
	{
		if (TSubclassOf<UUserWidget> WidgetClass = SaveableComponent->InteractionWidgetClass)
		{
			CurrentInteractionWidget = CreateWidget(this, WidgetClass);
			// You would typically have an interface to pass the managed entity to the widget here.
			CurrentInteractionWidget->AddToViewport();
		}
	}
	


	// Switch the input mode to UI only.


	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}



void AWispPlayerController::EndManagingEntity()
{
	if (CurrentInteractionWidget)
	{
		CurrentInteractionWidget->RemoveFromParent();
		CurrentInteractionWidget = nullptr;
	}

	// Re-enable movement on the pawn.
	if (GetPawn())
	{
		GetPawn()->EnableInput(this);
	}

	ManagedEntities.Empty();

	// Switch input mode back to Game only.
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
