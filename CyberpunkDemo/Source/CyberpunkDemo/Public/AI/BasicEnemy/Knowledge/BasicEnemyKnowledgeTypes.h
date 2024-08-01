// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../AIKnowledgeTypes.h"
#include "BasicEnemyKnowledgeTypes.generated.h"

UENUM(BlueprintType)
enum class EBasicEnemyState : uint8
{
	None,
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

UENUM(BlueprintType, Blueprintable)
enum class EBasicEnemyBehaviour : uint8
{
	None,
	Idle,
	ReturnToSpawnPoint,
	Patrol,
	BlindInvestigation,
	Investigation,
	MoveToBetterPosition,
	QuickMeleeAttack,
	ThrowGrenade,
	MoveToCover,
	ShootFromCover,
	ThrowGrenadeFromCover,
	Max UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct CYBERPUNKDEMO_API FBasicEnemyPersonalKnowledge
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettablePawn Agent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableVector	AgentLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableVector AgentSpawnLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableRotator AgentSpawnRotation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableFloat AgentDistanceFromSpawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	EBasicEnemyState AgentState = EBasicEnemyState::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	EBasicEnemyBehaviour AgentBehaviour = EBasicEnemyBehaviour::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableSpline PatrolSpline;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<FVector>	PatrolWaypoints;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableMainCharacter PlayerInSightCone;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableFloat DistanceFromPlayer;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableVector	SensedLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableVector CoverLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FSettableVector MoveToLocation;
};
