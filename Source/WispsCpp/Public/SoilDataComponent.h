#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h" // Our structs

#include "SoilDataComponent.generated.h"




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WISPSCPP_API USoilDataComponent : public UActorComponent
{

	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoilDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soil Data")
    FSoilData SoilData;
};

