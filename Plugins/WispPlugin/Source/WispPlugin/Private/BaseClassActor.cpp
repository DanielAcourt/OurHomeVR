// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClassActor.h"
#include "SaveableEntityComponent.h" // Include the new component headers
#include "QiComponent.h"

// Sets default values
ABaseClassActor::ABaseClassActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseClassActor::BeginPlay()
{
	Super::BeginPlay();

	//PrimaryActorTick.bCanEverTick = true;
	
}

// Called every frame
// You can remove the Tick and SetupPlayerInputComponent functions if they are empty.

/*
void ABaseClassActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
