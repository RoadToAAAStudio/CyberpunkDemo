// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/QuickhackSystemComponent.h"

#include "NavigationSystemTypes.h"


// Sets default values for this component's properties
UQuickhackSystemComponent::UQuickhackSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UQuickhackSystemComponent::Inspect()
{
	FVector LineStart = GetOwner()->GetActorLocation();
	FVector LineEnd = GetOwner()->GetActorForwardVector() * 1000;
	FHitResult HitResult;
	IHackerable* HackerableTarget = nullptr;
	if (GetWorld()->LineTraceSingleByProfile(HitResult, LineStart, LineEnd, "BlockAll", Params))
	{
		if (Cast<IHackerable>(HitResult.GetActor()))
		{
			CreateHacks(Cast<IHackerable>(HitResult.GetActor()));
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green, false, 2);
		}
		else
		{
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 2);
		}
	}
}

void UQuickhackSystemComponent::CreateHacks(const IHackerable* HackTarget)
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks = HackTarget->GetPossibleHacks();
	for (auto Hack : PossibleHacks)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString(Hack->GetClass()->GetName()));
	}
	
}


// Called when the game starts
void UQuickhackSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuickhackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

