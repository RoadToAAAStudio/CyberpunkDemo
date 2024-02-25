// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Doors/UnlockCondition.h"


// Sets default values
AUnlockCondition::AUnlockCondition()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AUnlockCondition::Interact()
{
	return false;
}

// Called when the game starts or when spawned
void AUnlockCondition::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnlockCondition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

