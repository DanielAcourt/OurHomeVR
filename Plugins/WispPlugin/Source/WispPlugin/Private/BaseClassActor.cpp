// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClassActor.h"
#include "SaveableEntityComponent.h"
#include "QiComponent.h"

// Sets default values
ABaseClassActor::ABaseClassActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SaveableEntityComponent = CreateDefaultSubobject<USaveableEntityComponent>(TEXT("SaveableEntityComponent"));
	QiComponent = CreateDefaultSubobject<UQiComponent>(TEXT("QiComponent"));
}

// Called when the game starts or when spawned
void ABaseClassActor::BeginPlay()
{
	Super::BeginPlay();
}
