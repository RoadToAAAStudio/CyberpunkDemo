// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/HackableObjects/BaseHackerable.h"


// Sets default values
ABaseHackerable::ABaseHackerable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TSet<TSubclassOf<UGameplayAbility>> ABaseHackerable::GetPossibleHacks() const
{
	return PossibleHacks;
}

void ABaseHackerable::Highlight() const
{
	IHackerable::Highlight();
}

// Called when the game starts or when spawned
void ABaseHackerable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseHackerable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

