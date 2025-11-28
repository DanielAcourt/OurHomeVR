#include "GridDataComponent.h"

UGridDataComponent::UGridDataComponent()
{
	// This component doesn't need to tick. The manager will handle updates.
	PrimaryComponentTick.bCanEverTick = false;
}