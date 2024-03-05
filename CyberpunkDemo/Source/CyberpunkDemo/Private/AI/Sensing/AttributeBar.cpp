// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Sensing/AttributeBar.h"

// Sets default values for this component's properties
UAttributeBar::UAttributeBar()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeBar::AddAmount(float Amount)
{
	if (Amount < 0 || CurrentValue == 1) return;
	CurrentValue += Amount;
	CurrentValue = FMath::Clamp(CurrentValue, 0.0f, 1.0f);

	if (CurrentValue == 1)
	{
		bIsFull = true;
	}
	if (CurrentValue == 1)
	{
		OnBarFilledDelegate.Broadcast();
	}
}

void UAttributeBar::RemoveAmount(float Amount)
{
	if (Amount < 0 || CurrentValue == 0) return;
	CurrentValue -= Amount;
	CurrentValue = FMath::Clamp(CurrentValue, 0.0f, 1.0f);

	if (CurrentValue < 1)
	{
		bIsFull = false;
	}
	if (CurrentValue == 0)
	{
		OnBarEmptiedDelegate.Broadcast();
	}
}

void UAttributeBar::Fill()
{
	CurrentValue = 1;
	bIsFull = true;
}

void UAttributeBar::Reset()
{
	CurrentValue = 0;
	bIsFull = false;
}


// Called when the game starts
void UAttributeBar::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAttributeBar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

