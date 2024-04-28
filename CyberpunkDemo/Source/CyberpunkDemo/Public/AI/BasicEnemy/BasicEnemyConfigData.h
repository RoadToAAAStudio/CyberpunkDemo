// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BasicEnemyConfigData.generated.h"

enum class EBasicEnemyGoalType : uint8;

USTRUCT(BlueprintType)
struct FActuatorsConfigData : public FTableRowBase
{
	GENERATED_BODY()
	// TODO
};

USTRUCT(BlueprintType)
struct FBrainConfigData : public FTableRowBase
{
	GENERATED_BODY()
	// TODO
};

USTRUCT(BlueprintType)
struct FKnowledgeConfigData : public FTableRowBase
{
	GENERATED_BODY()

	// Perception Configs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightRadius						= 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightLoseRadius					= 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPeripheralVisionAngleDegrees = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightNearClippingRadius			= 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightPointOfViewBackwardOffset	= 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingRange						= 100.0f;

	// Sensors Configs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseIncreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightBaseDecreaseRate				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightCrouchMultiplier				= 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMinMultiplier	    = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float SightDistanceMaxMultiplier	    = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	float HearingBaseDecreaseRate		    = 1.0f;

	// Goals Generation
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TArray<EBasicEnemyGoalType> SupportedGoals;
};

USTRUCT(BlueprintType)
struct FBasicEnemyConfigData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  TObjectPtr<UDataTable>  KnowledgeConfigData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)  TObjectPtr<UDataTable>  BrainConfigData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)  TObjectPtr<UDataTable>  ActuatorsConfigData;
};


