// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/SplineContainer.h"

// Sets default values
ASplineContainer::ASplineContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>("SplinePath");
	RootComponent = Spline;
}

// Called when the game starts or when spawned
void ASplineContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

