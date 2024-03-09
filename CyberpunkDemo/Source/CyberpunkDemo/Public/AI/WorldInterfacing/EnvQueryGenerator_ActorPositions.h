// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EnvQueryGenerator_ActorPositions.generated.h"

/**
 * Generates Actors of a Class Positions
 */
UCLASS(meta = (DisplayName = "Actor Of Class Positions"))
class CYBERPUNKDEMO_API UEnvQueryGenerator_ActorPositions : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Generator, meta=(AllowAbstract))
	TSubclassOf<AActor> SearchedActorClass;

	/** If true, this will only return actors of the specified class within the SearchRadius of the SearchCenter context.  If false, it will return ALL actors of the specified class in the world. */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderBoolValue GenerateOnlyActorsInRadius;

	/** Max distance of path between point and context.  NOTE: Zero and negative values will never return any results if
	  * UseRadius is true.  "Within" requires Distance < Radius.  Actors ON the circle (Distance == Radius) are excluded.
	  */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue SearchRadius;

	/** context */
	UPROPERTY(EditAnywhere, Category=Generator)
	TSubclassOf<UEnvQueryContext> SearchCenter;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
