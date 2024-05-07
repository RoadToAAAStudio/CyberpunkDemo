// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BasicEnemyConfigData.generated.h"

class UGoalGenerator;
enum class EBasicEnemyGoalType : uint8;

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

UCLASS(Blueprintable)
class CYBERPUNKDEMO_API UBasicEnemyConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Knowledge | Sensors")
	FSensorsConfigData SensorsConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Knowledge | Perception")
	FPerceptionConfigData PerceptionConfigData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Knowledge | Goals")
	TArray<TSubclassOf<UGoalGenerator>> SupportedGoals;
};


