#include "InteractionComponent.h"
#include "Interactable.h"
#include "SaveableEntityComponent.h"
#include "QiComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CamLoc;
	FRotator CamRot;

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (!MyPawn) return;
	AController* MyController = MyPawn->GetController();
	if (!MyController) return;

	MyController->GetPlayerViewPoint(CamLoc, CamRot);

	FVector Start = CamLoc;
	FVector End = Start + (CamRot.Vector() * InteractionDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	AActor* NewFocusedActor = nullptr;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		NewFocusedActor = HitResult.GetActor();
	}

	if (NewFocusedActor != FocusedActor)
	{
		if (FocusedActor && FocusedActor->Implements<UInteractable>())
		{
			IInteractable::Execute_OnEndHover(FocusedActor);
		}
		if (NewFocusedActor && NewFocusedActor->Implements<UInteractable>())
		{
			IInteractable::Execute_OnBeginHover(NewFocusedActor);
		}
		FocusedActor = NewFocusedActor;
	}
	
	// --- BEGIN DEBUG DISPLAY LOGIC ---
	if (bEnableDebugDisplay && FocusedActor)
	{
		FString DebugString = FString::Printf(TEXT("Name: %s"), *FocusedActor->GetName());

		if (USaveableEntityComponent* SaveComp = FocusedActor->FindComponentByClass<USaveableEntityComponent>())
		{
			DebugString += TEXT(""); // Add a new line
				DebugString += FString::Printf(TEXT("EntityType: %s"), *SaveComp->EntityTypeTag.ToString());
		}

		if (UQiComponent* QiComp = FocusedActor->FindComponentByClass<UQiComponent>())
		{
			DebugString += TEXT(""); // Add a new line
				DebugString += FString::Printf(TEXT("Qi: %.1f / %.1f"), QiComp->QiData.CurrentQi, QiComp->QiData.MaxQi);
		}

		DrawDebugString(GetWorld(), FocusedActor->GetActorLocation(), DebugString, nullptr, FColor::White, 0.0f, true);
	}
	// --- END DEBUG DISPLAY LOGIC ---
}



void UInteractionComponent::Interact()
{
	if (FocusedActor && FocusedActor->Implements<UInteractable>())
	{
		IInteractable::Execute_OnInteract(FocusedActor);
	}
}

