// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "UObject/Object.h"
#include "BasicEnemyConfigData.generated.h"

class UStateTree;

USTRUCT(BlueprintType)
struct FPerceptionConfigData
{
	GENERATED_BODY()

	// Perception Configs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightRadius						= 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightLoseRadius					= 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPeripheralVisionAngleDegrees = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightNearClippingRadius			= 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPointOfViewBackwardOffset	= 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingRange						= 100.0f;
};

USTRUCT(BlueprintType)
struct FSensorsConfigData
{
	GENERATED_BODY()

	// Sensors Configs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseIncreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseDecreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightCrouchMultiplier				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMinMultiplier	    = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMaxMultiplier	    = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingBaseDecreaseRate		    = 1.0f;
};

// Perception Configs
UCLASS(Blueprintable)
class CYBERPUNKDEMO_API UBasicEnemyPerceptionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightRadius						= 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightLoseRadius					= 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPeripheralVisionAngleDegrees = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightNearClippingRadius			= 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPointOfViewBackwardOffset	= 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingRange						= 100.0f;
};

// Sensors Configs
UCLASS(Blueprintable)
class CYBERPUNKDEMO_API UBasicEnemySensorsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseIncreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseDecreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightCrouchMultiplier				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMinMultiplier	    = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMaxMultiplier	    = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingBaseDecreaseRate		    = 1.0f;
};

// Brain Configs
UCLASS(Blueprintable)
class CYBERPUNKDEMO_API UBasicEnemyBrainData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Brain | State Machine")	FStateTreeReference Brain;
	UPROPERTY(EditAnywhere, Category="Brain | States")			TMap<EBasicEnemyState, FStateTreeReference> States;
	UPROPERTY(EditAnywhere, Category="Brain | Behaviours")		TMap<EBasicEnemyBehaviour, FStateTreeReference> Behaviours;
};

// Archetype Configs
UCLASS(Blueprintable, BlueprintType)
class CYBERPUNKDEMO_API UBasicEnemyConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Knowledge | Perception")
	TObjectPtr<UBasicEnemyPerceptionData> PerceptionConfigData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Knowledge | Sensors")
	TObjectPtr<UBasicEnemySensorsData> SensorsConfigData;
	
	UPROPERTY(EditAnywhere, Category="Brain")
	TObjectPtr<UBasicEnemyBrainData> BrainConfigData;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UStateTree* GetDefaultBehaviour() const
	{
		if (BrainConfigData != nullptr)
		{
			return BrainConfigData->Brain.GetStateTree();
		}

		return nullptr;
	}
};


