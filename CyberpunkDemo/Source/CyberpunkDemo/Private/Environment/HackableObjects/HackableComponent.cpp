// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/HackableObjects/HackableComponent.h"


// Sets default values for this component's properties
UHackableComponent::UHackableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

TSet<TSubclassOf<UGameplayAbility>> UHackableComponent::GetPossibleHacks() const
{
	TSet<TSubclassOf<UGameplayAbility>> Hacks;
	FObjectTypeDataStructure* ObjectTypeData = ObjectData->FindRow<FObjectTypeDataStructure>(FName(UEnum::GetValueAsString(ObjectType)), "");
	if (!ObjectTypeData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Object type data not found");
		return Hacks;
	}
	Hacks = ObjectTypeData->PossibleHacks;
	return ObjectTypeData->PossibleHacks;
}

#pragma region TIMER_HANDLE
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

void UHackableComponent::SetHasBeenInspected()
{
	bHasBeenInspected = true;
	bIsUnderInspection = false;
}

#pragma endregion

void UHackableComponent::Highlight() const
{
	
}

// Called when the game starts
void UHackableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (static_cast<int>(ObjectType) == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString(GetName() + "has not an OBJECT TYPE SPECIFIED!"));
	}
}

// Called every frame
void UHackableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

