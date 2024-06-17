// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/EnvQueryTest_ClaimableLocations.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"


UEnvQueryTest_ClaimableLocations::UEnvQueryTest_ClaimableLocations(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	SetWorkOnFloatValues(false);

	// To search for GameplayTags, currently we require the item type to be an actor.  Certainly it must at least be a
	// class of some sort to be able to find the interface required.
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

void UEnvQueryTest_ClaimableLocations::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}
	
	TArray<AActor*> ContextActors;
	if (!QueryInstance.PrepareContext(OwnerContext, ContextActors))
	{
		return;
	}
	
	if (ContextActors.Num() <= 0 || ContextActors.Num() > 1)
	{
		return;
	}
	
	BoolValue.BindData(QueryOwner, QueryInstance.QueryID);
	bool bWantsValid = BoolValue.GetValue();
	
	// loop through all items
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		if (const IClaimable* ClaimableObject = Cast<const IClaimable>(ItemActor))
		{
			bool bSatisfiesTest = ClaimableObject->IsClaimable_Implementation(ContextActors[0]);
	
			// bWantsValid is the basically the opposite of bInverseCondition in BTDecorator.  Possibly we should
			// rename to make these more consistent.
			It.SetScore(TestPurpose, FilterType, bSatisfiesTest, bWantsValid);
		}
		else 
		{
			It.ForceItemState(EEnvItemStatus::Failed);
		}
	}
}

FText UEnvQueryTest_ClaimableLocations::GetDescriptionTitle() const
{
	return Super::GetDescriptionTitle();
}

FText UEnvQueryTest_ClaimableLocations::GetDescriptionDetails() const
{
	return Super::GetDescriptionDetails();
}
