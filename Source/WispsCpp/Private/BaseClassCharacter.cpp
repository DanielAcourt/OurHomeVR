// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClassCharacter.h"
#include "SaveableEntityComponent.h" // Include the new component headers
#include "QiComponent.h"

//example to make a function

// Sets default values
ABaseClassCharacter::ABaseClassCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 

	// Create the core components.
	SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));
	QiComponent = CreateDefaultSubobject<UQiComponent>(TEXT("QiComponent"));

}

// Called when the game starts or when spawned
void ABaseClassCharacter::BeginPlay()
{
	Super::BeginPlay();

	//PrimaryActorTick.bCanEverTick = true;
	
}

// You can remove the Tick and SetupPlayerInputComponent functions if they are empty.
/*
// Called every frame
void ABaseClassCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseClassCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/
