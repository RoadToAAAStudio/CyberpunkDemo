// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/WorldInterfacing/EnvQueryGenerator_ActorPositions.h"
#include "EngineUtils.h"
#include "GameplayTagContainer.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EnvQueryGenerator_ActorPositions)

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_ActorPositions::UEnvQueryGenerator_ActorPositions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemType = UEnvQueryItemType_Point::StaticClass();

	SearchedActorClass = AActor::StaticClass();
	GenerateOnlyActorsInRadius.DefaultValue = true;
	SearchRadius.DefaultValue = 500.0f;
	SearchCenter = UEnvQueryContext_Querier::StaticClass();
}

void UEnvQueryGenerator_ActorPositions::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	if (SearchedActorClass == nullptr)
	{
		return;
	}

	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(QueryOwner, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}

	GenerateOnlyActorsInRadius.BindData(QueryOwner, QueryInstance.QueryID);
	bool bUseRadius = GenerateOnlyActorsInRadius.GetValue();

	TArray<FNavLocation> MatchingActorsLocations;
	if (bUseRadius)
	{
		TArray<FVector> ContextLocations;
		QueryInstance.PrepareContext(SearchCenter, ContextLocations);

		SearchRadius.BindData(QueryOwner, QueryInstance.QueryID);
		const float RadiusValue = SearchRadius.GetValue();
		const float RadiusSq = FMath::Square(RadiusValue);

		for (TActorIterator<AActor> ItActor = TActorIterator(World, SearchedActorClass); ItActor; ++ItActor)
		{
			if (!TagQueryToMatch.IsEmpty() && !SatisfiesTest(Cast<IGameplayTagAssetInterface>(*ItActor)))
			{
				continue;
			}
			
			for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ++ContextIndex)
			{
				if (FVector::DistSquared(ContextLocations[ContextIndex], ItActor->GetActorLocation()) < RadiusSq)
				{
					MatchingActorsLocations.Add(FNavLocation((*ItActor)->GetActorLocation()));
					break;
				}
			}
		}
	}
	else
	{
		// If radius is not positive, ignore Search Center and Search Radius and just return all actors of class.
		for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(World, SearchedActorClass); ItActor; ++ItActor)
		{
			MatchingActorsLocations.Add(FNavLocation((*ItActor)->GetActorLocation()));
		}
	}
	
	ProjectAndFilterNavPoints(MatchingActorsLocations, QueryInstance);
	StoreNavPoints(MatchingActorsLocations, QueryInstance);
}

bool UEnvQueryGenerator_ActorPositions::SatisfiesTest(const IGameplayTagAssetInterface* ItemGameplayTagAssetInterface) const
{
	// Currently we're requiring that the test only be run on items that implement the interface.  In theory, we could
	// (instead of checking) support correctly passing or failing on items that don't implement the interface or
	// just have a nullptr item by testing them as though they have the interface with no gameplay tags.  However, that
	// seems potentially confusing, since certain tests could actually pass.
	check(ItemGameplayTagAssetInterface != nullptr);
	
	FGameplayTagContainer OwnedGameplayTags;
	ItemGameplayTagAssetInterface->GetOwnedGameplayTags(OwnedGameplayTags);

	return OwnedGameplayTags.MatchesQuery(TagQueryToMatch);
}

FText UEnvQueryGenerator_ActorPositions::GetDescriptionTitle() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("DescriptionTitle"), Super::GetDescriptionTitle());
	Args.Add(TEXT("ActorsClass"), FText::FromString(GetNameSafe(SearchedActorClass)));

	if (!GenerateOnlyActorsInRadius.IsDynamic() && !GenerateOnlyActorsInRadius.GetValue())
	{
		return FText::Format(LOCTEXT("DescriptionGenerateActors", "{DescriptionTitle}: generate set of actors locations of {ActorsClass}"), Args);
	}

	Args.Add(TEXT("DescribeContext"), UEnvQueryTypes::DescribeContext(SearchCenter));
	return FText::Format(LOCTEXT("DescriptionGenerateActorsAroundContext", "{DescriptionTitle}: generate set of actors locations of {ActorsClass} around {DescribeContext}"), Args);

}

FText UEnvQueryGenerator_ActorPositions::GetDescriptionDetails() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Radius"), FText::FromString(SearchRadius.ToString()));

	FText Desc = FText::Format(LOCTEXT("ActorsOfClassDescription", "radius: {Radius}"), Args);

	return Desc;
}

#undef LOCTEXT_NAMESPACE
