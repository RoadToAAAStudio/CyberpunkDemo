// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WorldInterfacing/Location.h"

#include "Components/SphereComponent.h"

// Sets default values
ALocation::ALocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	RootComponent = SphereTrigger;
}

// Called when the game starts or when spawned
void ALocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALocation::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTagContainer);
}

