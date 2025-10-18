#include "WispPawn.h"
#include "Components/StaticMeshComponent.h"

AWispPawn::AWispPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// Make the default mesh invisible.
	/*
	if (GetMeshComponent())
	{
		GetMeshComponent()->bVisible = false;
	}
	*/
}