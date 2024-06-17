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

void ALocation::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTagContainer);
}

bool ALocation::Claim_Implementation(AActor* NewOwner)
{
	if (!NewOwner) return false;
	if (IsUsedBy_Implementation(NewOwner)) return true;
	if (IsUsed_Implementation()) return false;

	ClaimOwner = NewOwner;
	GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Environment.Object.Used")));
	return true;
}

bool ALocation::Release_Implementation(const AActor* OldOwner)
{
	if (!OldOwner || OldOwner != ClaimOwner) return false;
	
	ClaimOwner = nullptr;
	GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Environment.Object.Used")));
	return true;
}

bool ALocation::IsUsed_Implementation() const
{
	if (ClaimOwner != nullptr && GameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Environment.Object.Used"))))
	{
		return true;
	}

	return false;
}

bool ALocation::IsUsedBy_Implementation(const AActor* PotentialOwner) const
{
	if (IsUsed_Implementation() && ClaimOwner == PotentialOwner)
	{
		return true;
	}
	
	return false;
}

bool ALocation::IsClaimable_Implementation(const AActor* PotentialOwner) const
{
	if (IsUsedBy_Implementation(PotentialOwner)) return true;
	if (!IsUsed_Implementation()) return true;
	
	return false;
}

