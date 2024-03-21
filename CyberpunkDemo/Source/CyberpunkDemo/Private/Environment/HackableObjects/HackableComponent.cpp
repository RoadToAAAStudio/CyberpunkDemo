// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/HackableObjects/HackableComponent.h"


// Sets default values for this component's properties
UHackableComponent::UHackableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

TSet<TSubclassOf<UGameplayAbility>> UHackableComponent::GetPossibleHacks() const
{
	return PossibleHacks;
}

void UHackableComponent::StartInspectionTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(InspectionTimerHandle))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(InspectionTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(InspectionTimerHandle, this, &UHackableComponent::SetHasBeenInspected, InspectionTime, false);
	}
	bIsUnderInspection = true;
}

void UHackableComponent::StopInspectionTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(InspectionTimerHandle);
	bIsUnderInspection = false;
}

bool UHackableComponent::GetHasBeenInspected()
{
	return bHasBeenInspected;
}

bool UHackableComponent::GetIsUnderInspection()
{
	return bIsUnderInspection;
}

// void UHackableComponent::SetIsUnderInspection(bool UnderInspection)
// {
// 	bIsUnderInspection = UnderInspection;
// }

void UHackableComponent::Highlight() const
{
	
}


// Called when the game starts
void UHackableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UHackableComponent::SetHasBeenInspected()
{
	bHasBeenInspected = true;
	bIsUnderInspection = false;
}

// Called every frame
void UHackableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if (InspectionTimerHandle.IsValid())
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString(FString::SanitizeFloat(GetWorld()->GetTimerManager().GetTimerRemaining(InspectionTimerHandle))));
}

