// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AIKnowledgeTypes.h"
#include "AIZoneKnowledgeTypes.generated.h"

class ABasicEnemy;
class ALocation;

UENUM(BlueprintType)
enum class EAIZoneState : uint8
{
	None,
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct FBasicEnemySharedKnowledge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettablePawn Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableTimerHandle CombatTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableFloat CombatTimerDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableTimerHandle AlertedTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableFloat AlertedTimerDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSettableInt NumberOfSightConesThePlayerIsIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector, TObjectPtr<ALocation>> CoverPerLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIZoneState AIZoneState = EAIZoneState::None;
};
