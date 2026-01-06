#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QiComponent.generated.h"

USTRUCT(BlueprintType)
struct FQiData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
    float CurrentQi;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
    float MaxQi;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    float CurrentPollen;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    float MaxPollen;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WISPPLUGIN_API UQiComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQiComponent();

	UFUNCTION(BlueprintCallable, Category = "Resources")
	float ConsumePollen(float AmountToTake);

	UFUNCTION(BlueprintCallable, Category = "Qi")
	void ConsumeQi(float AmountToConsume);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
	FQiData QiData;
};
