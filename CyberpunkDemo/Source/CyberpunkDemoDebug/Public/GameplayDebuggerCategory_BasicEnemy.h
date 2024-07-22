// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#if WITH_GAMEPLAY_DEBUGGER
#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

enum class EAIZoneState : uint8;
enum class EBasicEnemyState : uint8;
enum class EBasicEnemyBehaviour : uint8;
class ABasicEnemy;
class ALocation;

class FGameplayDebuggerCategory_BasicEnemy : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_BasicEnemy();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FRepData
	{
		// SharedKnowledge
		FString PlayerName;
		FVector PlayerLocation;
		float CombatTimerValue;
		float AlertedTimerValue;
		int NumberOfSightConePlayerIsIn;
		TArray<FString> Enemies;
		TArray<FVector> Covers;
		EAIZoneState AIZoneState;
		
		// Personal Knowledge
		FString AgentName;
		FVector AgentLocation;
		FVector AgentSpawnLocation;
		FRotator AgentSpawnRotation;
		float AgentDistanceFormSpawn;
		EBasicEnemyState AgentState;
		EBasicEnemyBehaviour AgentBehaviour;
		FString PatrolSplineName;
		TArray<FVector> PatrolWaypoints;
		FString PlayerInSightConeName;
		float DistanceFromPlayer;
		FVector SensedLocation;
		FVector CoverLocation;
		FVector MoveToLocation;
		
		// Sensors
		float SightValue;
		float HearingValue;

		// Shared Knowledge
		uint32 bIsPlayerSet : 1;
		uint32 bIsPlayerLocationSet : 1;
		uint32 bIsCombatTimerSet : 1;
		uint32 bIsAlertedTimerSet : 1;
		uint32 bIsNumberOfSightConesThePlayerIsInSet : 1;

		// Personal Knowledge
		uint32 bIsAgentSet : 1;
		uint32 bIsAgentLocationSet : 1;
		uint32 bIsAgentSpawnLocationSet : 1;
		uint32 bIsAgentSpawnRotationSet : 1;
		uint32 bIsAgentDistanceFromSpawnSet : 1;
		uint32 bIsPatrolSplineSet : 1;
		uint32 bIsPlayerInSightConeSet : 1;
		uint32 bIsDistanceFromPlayerSet : 1;
		uint32 bIsSensedLocationSet : 1;
		uint32 bIsCoverLocationSet : 1;
		uint32 bIsMoveToLocationSet : 1;

		// Sensors
		uint32 bIsSightEnabled : 1;
		uint32 bIsHearingEnabled : 1;

		void Serialize(FArchive& Ar);
	};
	FRepData DataPack;
};
#endif