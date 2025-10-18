#include "SaveableEntityComponent.h"
#include "Misc/Guid.h"

USaveableEntityComponent::USaveableEntityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveableEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// If this entity doesn't have a valid ID yet, create one.
	// This is crucial for new objects placed in the world.
	// For loaded objects, this ID will be overwritten by the save data.
	if (!InstanceID.IsValid())
	{
		InstanceID = FGuid::NewGuid();
	}

	// Here you would add logic to register this component with the GardenManager singleton
	// Example: UGardenManager::Get()->RegisterEntity(GetOwner());
}
