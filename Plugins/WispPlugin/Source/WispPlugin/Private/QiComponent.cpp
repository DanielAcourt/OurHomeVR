#include "QiComponent.h"

UQiComponent::UQiComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	QiData.CurrentQi = 1.0f;
}

float UQiComponent::ConsumePollen(float AmountToTake)
{
	float AmountTaken = FMath::Min(AmountToTake, QiData.CurrentPollen);
	QiData.CurrentPollen -= AmountTaken;
	return AmountTaken;
}

void UQiComponent::ConsumeQi(float AmountToConsume)
{
	QiData.CurrentQi = FMath::Max(0.0f, QiData.CurrentQi - AmountToConsume);
}
