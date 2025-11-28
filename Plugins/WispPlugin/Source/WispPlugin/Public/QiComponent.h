#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardenData.h" // Make sure this is included
#include "QiComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WISPPLUGIN_API UQiComponent : public UActorComponent
{
	GENERATED_BODY()
 
public:	
	// Sets default values for this component's properties
	UQiComponent();


public:	

		// The component now holds a single struct containing all its data.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Qi")
		FQiData QiData;

	protected:
		virtual void BeginPlay() override;
	};

