#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// This function is called by the Player Character when the interact key is pressed.
	UFUNCTION(BlueprintCallable)
	void Interact();

	// Change it to this:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> FocusedActor;

protected:
	virtual void BeginPlay() override;

private:
	// The distance the player can interact from.
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 500.f;

	// If true, will draw debug text in the world for the focused actor.
	UPROPERTY(EditAnywhere, Category = "Interaction|Debug")
	bool bEnableDebugDisplay = false;
};