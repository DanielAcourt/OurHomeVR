#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/**
 * @class UInteractionComponent
 * @brief Handles the player's ability to interact with objects in the world.
 *
 * This component traces for interactable objects in front of the player and
 * manages the currently focused actor.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPPLUGIN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UInteractionComponent();

	/**
	 * @brief Called every frame.
	 * @param DeltaTime The time since the last frame.
	 * @param TickType The type of tick.
	 * @param ThisTickFunction This tick function.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief This function is called by the Player Character when the interact key is pressed.
	 */
	UFUNCTION(BlueprintCallable)
	void Interact();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> FocusedActor;

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief The distance the player can interact from.
	 */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 500.f;

	/**
	 * @brief If true, will draw debug text in the world for the focused actor.
	 */
	UPROPERTY(EditAnywhere, Category = "Interaction|Debug")
	bool bEnableDebugDisplay = false;
};