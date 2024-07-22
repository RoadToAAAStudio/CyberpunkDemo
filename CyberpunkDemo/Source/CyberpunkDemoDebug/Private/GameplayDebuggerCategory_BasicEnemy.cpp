// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayDebuggerCategory_BasicEnemy.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "CyberpunkDemo/DebugMacros.h"
#include "AI/Utility/SplineContainer.h"
#include "MainCharacter/MainCharacter.h"
#include "AI/AIZone/AIZone.h"

FGameplayDebuggerCategory_BasicEnemy::FGameplayDebuggerCategory_BasicEnemy()
{
	SetDataPackReplication<FRepData>(&DataPack);
	bShowOnlyWithDebugActor = true;
}

void FGameplayDebuggerCategory_BasicEnemy::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (ABasicEnemy* BasicEnemy = Cast<ABasicEnemy>(DebugActor))
	{
		if (AAIZone* AIZone = BasicEnemy->AIZone)
		{
			// Shared Knowledge
			{
				const FBasicEnemySharedKnowledge& SharedKnowledge = AIZone->SharedKnowledge;

				DataPack.bIsPlayerSet = SharedKnowledge.Player.IsSet();
				DataPack.bIsPlayerLocationSet = SharedKnowledge.PlayerLocation.IsSet();
				DataPack.bIsCombatTimerSet = SharedKnowledge.CombatTimer.IsSet();
				DataPack.bIsAlertedTimerSet = SharedKnowledge.AlertedTimer.IsSet();
				DataPack.bIsNumberOfSightConesThePlayerIsInSet = SharedKnowledge.NumberOfSightConesThePlayerIsIn.IsSet();

				DataPack.PlayerName = DataPack.bIsPlayerSet ? SharedKnowledge.Player.Get()->GetActorNameOrLabel() : TEXT("null");
				DataPack.PlayerLocation = SharedKnowledge.PlayerLocation.Get();
				DataPack.CombatTimerValue = BasicEnemy->GetWorldTimerManager().GetTimerRemaining(SharedKnowledge.CombatTimer.Get());
				DataPack.AlertedTimerValue = BasicEnemy->GetWorldTimerManager().GetTimerRemaining(SharedKnowledge.AlertedTimer.Get());
				DataPack.NumberOfSightConePlayerIsIn = SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get();
				for (int i = 0; i < SharedKnowledge.Enemies.Num(); i++)
				{
					if (ABasicEnemy* AIZoneEnemy = SharedKnowledge.Enemies[i])
					{
						DataPack.Enemies.Add(AIZoneEnemy->GetActorNameOrLabel());
					}
				}
				SharedKnowledge.CoverPerLocations.GetKeys(DataPack.Covers);
				DataPack.AIZoneState = SharedKnowledge.AIZoneState;
			}
		}

		if (ABasicEnemyController* BasicEnemyController = BasicEnemy->BasicEnemyController)
		{
			if (UBasicEnemyKnowledgeComponent* KnowledgeComponent = BasicEnemyController->KnowledgeComponent)
			{
				// Personal Knowledge
				{
					const FBasicEnemyPersonalKnowledge& PersonalKnowledge = KnowledgeComponent->PersonalKnowledge;

					DataPack.bIsAgentSet = PersonalKnowledge.Agent.IsSet();
					DataPack.bIsAgentLocationSet = PersonalKnowledge.AgentLocation.IsSet();
					DataPack.bIsAgentSpawnLocationSet = PersonalKnowledge.AgentSpawnLocation.IsSet();
					DataPack.bIsAgentSpawnRotationSet = PersonalKnowledge.AgentSpawnRotation.IsSet();
					DataPack.bIsAgentDistanceFromSpawnSet = PersonalKnowledge.AgentDistanceFromSpawn.IsSet();
					DataPack.bIsPatrolSplineSet = PersonalKnowledge.PatrolSpline.IsSet();
					DataPack.bIsPlayerInSightConeSet = PersonalKnowledge.PlayerInSightCone.IsSet();
					DataPack.bIsDistanceFromPlayerSet = PersonalKnowledge.DistanceFromPlayer.IsSet();
					DataPack.bIsSensedLocationSet = PersonalKnowledge.SensedLocation.IsSet();
					DataPack.bIsCoverLocationSet = PersonalKnowledge.CoverLocation.IsSet();
					DataPack.bIsMoveToLocationSet = PersonalKnowledge.MoveToLocation.IsSet();

					DataPack.AgentName = DataPack.bIsAgentSet ? PersonalKnowledge.Agent.Get()->GetActorNameOrLabel() : TEXT("null");
					DataPack.AgentLocation = PersonalKnowledge.AgentLocation.Get();
					DataPack.AgentSpawnLocation = PersonalKnowledge.AgentSpawnLocation.Get();
					DataPack.AgentSpawnRotation = PersonalKnowledge.AgentSpawnRotation.Get();
					DataPack.AgentDistanceFormSpawn = PersonalKnowledge.AgentDistanceFromSpawn.Get();
					DataPack.AgentState = PersonalKnowledge.AgentState;
					DataPack.AgentBehaviour = PersonalKnowledge.AgentBehaviour;
					DataPack.PatrolSplineName = DataPack.bIsPatrolSplineSet ? PersonalKnowledge.PatrolSpline.Get()->Spline->GetName() : TEXT("null");
					DataPack.PatrolWaypoints = PersonalKnowledge.PatrolWaypoints;
					DataPack.PlayerInSightConeName = DataPack.bIsPlayerInSightConeSet ? PersonalKnowledge.PlayerInSightCone.Get()->GetActorNameOrLabel() : TEXT("null");
					DataPack.DistanceFromPlayer = PersonalKnowledge.DistanceFromPlayer.Get();
					DataPack.SensedLocation = PersonalKnowledge.SensedLocation.Get();
					DataPack.CoverLocation = PersonalKnowledge.CoverLocation.Get();
					DataPack.MoveToLocation = PersonalKnowledge.MoveToLocation.Get();
				}

				// Sensors
				{
					DataPack.bIsSightEnabled = KnowledgeComponent->IsSightEnabled();
					DataPack.bIsHearingEnabled = KnowledgeComponent->IsHearingEnabled();
					DataPack.SightValue = KnowledgeComponent->GetSightBarValue();
					DataPack.HearingValue = KnowledgeComponent->GetHearingBarValue();
				}
			}
		}
	}
}

void FGameplayDebuggerCategory_BasicEnemy::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	// Shared Knowledge
	{
		const FString PlayerColorString = DataPack.bIsPlayerSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString PlayerLocationColorString = DataPack.bIsPlayerLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString CombatTimerColorString = DataPack.bIsCombatTimerSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString AlertedTimerColorString = DataPack.bIsAlertedTimerSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString NumberOfSightConesThePlayerIsInColorString = DataPack.bIsNumberOfSightConesThePlayerIsInSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		
		CanvasContext.MoveToNewLine();

		CanvasContext.Printf(TEXT("[SHARED_KNOWLEDGE]"));
		CanvasContext.Printf(TEXT("{yellow}Player: {%s}%s{yellow} - Location: {%s}(%.2f, %.2f, %.2f)"),
			*PlayerColorString, *(DataPack.PlayerName),
			*PlayerLocationColorString, DataPack.PlayerLocation.X, DataPack.PlayerLocation.Y, DataPack.PlayerLocation.Z);
		CanvasContext.Printf(TEXT("{yellow}Combat timer: {%s}%.2f{yellow} - Alerted timer: {%s}%.2f"),
			*CombatTimerColorString, DataPack.CombatTimerValue,
			*AlertedTimerColorString, DataPack.AlertedTimerValue);
		CanvasContext.Printf(TEXT("{yellow}NumberOfSightConesThePlayerIsIn: {%s}%d"),
			*NumberOfSightConesThePlayerIsInColorString, DataPack.NumberOfSightConePlayerIsIn);
		CanvasContext.Printf(TEXT("{yellow}NumberOfEnemies: {green}%d"), 
			DataPack.Enemies.Num());
		CanvasContext.Printf(TEXT("{yellow}NumberOfCovers: {green}%d"), 
			DataPack.Covers.Num());
		CanvasContext.Printf(TEXT("{yellow}AIZone State: {green}%s"), 
			*(UEnum::GetDisplayValueAsText(DataPack.AIZoneState).ToString()));
	}

	// Personal Knowledge
	{
		const FString AgentColorString = DataPack.bIsAgentSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString AgentLocationColorString = DataPack.bIsAgentLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString AgentSpawnLocationColorString = DataPack.bIsAgentSpawnLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString AgentSpawnRotationColorString = DataPack.bIsAgentSpawnRotationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString AgentDistanceFromSpawnColorString = DataPack.bIsAgentDistanceFromSpawnSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString PatrolSplineColorString = DataPack.bIsPatrolSplineSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString PlayerInSightConeColorString = DataPack.bIsPlayerInSightConeSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString DistanceFromPlayerColorString = DataPack.bIsDistanceFromPlayerSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString SensedLocationColorString = DataPack.bIsSensedLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString CoverLocationColorString = DataPack.bIsCoverLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString MoveToLocationColorString = DataPack.bIsMoveToLocationSet ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		
		CanvasContext.MoveToNewLine();

		CanvasContext.Printf(TEXT("[PERSONAL_KNOWLEDGE]"));
		CanvasContext.Printf(TEXT("{yellow}Agent: {%s}%s{yellow} - Location: {%s}(%.2f, %.2f, %.2f)}"), 
			*AgentColorString, *(DataPack.AgentName), 
			*AgentLocationColorString, DataPack.AgentLocation.X, DataPack.AgentLocation.Y, DataPack.AgentLocation.Z);
		CanvasContext.Printf(TEXT("{yellow}SpawnLocation: {%s}(%.2f, %.2f, %.2f) {yellow}- Rotation: ({%s}%.2f, %.2f, %.2f) {yellow}- Distance: {%s}%.2f"), 
			*AgentSpawnLocationColorString, DataPack.AgentSpawnLocation.X, DataPack.AgentSpawnLocation.Y, DataPack.AgentSpawnLocation.Z, 
			*AgentSpawnRotationColorString, DataPack.AgentSpawnRotation.Roll, DataPack.AgentSpawnRotation.Pitch, DataPack.AgentSpawnRotation.Yaw, 
			*AgentDistanceFromSpawnColorString, DataPack.AgentDistanceFormSpawn);
		CanvasContext.Printf(TEXT("{yellow}State: {green}%s"), 
			*(UEnum::GetDisplayValueAsText(DataPack.AgentState).ToString()));
		CanvasContext.Printf(TEXT("{yellow}Behaviour: {green}%s"),
			*(UEnum::GetDisplayValueAsText(DataPack.AgentBehaviour).ToString()));
		CanvasContext.Printf(TEXT("{yellow}PatrolSpline: {%s}%s"), 
			*PatrolSplineColorString, *(DataPack.PatrolSplineName));
		for (int i = 0; i < DataPack.PatrolWaypoints.Num(); i++)
		{
			const FVector PatrolWaypoint = DataPack.PatrolWaypoints[i];
			CanvasContext.Printf(TEXT("{yellow} - Waypoint (%d): %.2f, %.2f, %.2f"), (i + 1), PatrolWaypoint.X, PatrolWaypoint.Y, PatrolWaypoint.Z);
		}
		CanvasContext.Printf(TEXT("{yellow}Player InSightCone: {%s}%s {yellow}- Distance: {%s}%.2f"), 
			*PlayerInSightConeColorString, *(DataPack.PlayerInSightConeName),
			*DistanceFromPlayerColorString, DataPack.DistanceFromPlayer);
		CanvasContext.Printf(TEXT("{yellow}SensedLocation: {%s}(%.2f, %.2f, %.2f)"), 
			*SensedLocationColorString, DataPack.SensedLocation.X, DataPack.SensedLocation.Y, DataPack.SensedLocation.Z);
		CanvasContext.Printf(TEXT("{yellow}CoverLocation: {%s}(%.2f, %.2f, %.2f)"), 
			*CoverLocationColorString, DataPack.CoverLocation.X, DataPack.CoverLocation.Y, DataPack.CoverLocation.Z);
		CanvasContext.Printf(TEXT("{yellow}MoveToLocation: {%s}(%.2f, %.2f, %.2f)"), 
			*MoveToLocationColorString, DataPack.MoveToLocation.X, DataPack.MoveToLocation.Y, DataPack.MoveToLocation.Z);
	
	}

	// Sensors 
	{
		const FString SightColorString = DataPack.bIsSightEnabled ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;
		const FString HearingColorString = DataPack.bIsHearingEnabled ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled;

		CanvasContext.MoveToNewLine();

		CanvasContext.Printf(TEXT("[SENSOR_DATA]"));
		CanvasContext.Printf(TEXT("{yellow}SightValue: {%s}%.2f {yellow}- HearingValue: {%s}%.2f"), 
			*SightColorString, DataPack.SightValue, 
			*HearingColorString, DataPack.HearingValue);
	}

}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_BasicEnemy::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_BasicEnemy());;
}

void FGameplayDebuggerCategory_BasicEnemy::FRepData::Serialize(FArchive& Ar)
{
	// Shared Knoweldge
	Ar << PlayerName;
	Ar << PlayerLocation;
	Ar << CombatTimerValue;
	Ar << AlertedTimerValue;
	Ar << NumberOfSightConePlayerIsIn;
	Ar << Enemies;
	Ar << Covers;
	Ar << AIZoneState;
	
	// Personal Knowledge
	Ar << AgentName;
	Ar << AgentLocation;
	Ar << AgentSpawnLocation;
	Ar << AgentSpawnRotation;
	Ar << AgentDistanceFormSpawn;
	Ar << AgentState;
	Ar << AgentBehaviour;
	Ar << PatrolSplineName;
	Ar << PatrolWaypoints;
	Ar << PlayerInSightConeName;
	Ar << DistanceFromPlayer;
	Ar << SensedLocation;
	Ar << CoverLocation;
	Ar << MoveToLocation;
	Ar << SightValue;
	Ar << HearingValue;

	uint32 BitFlags =
		((bIsPlayerSet ? 1 : 0) << 0) |
		((bIsPlayerLocationSet ? 1 : 0) << 1) |
		((bIsCombatTimerSet ? 1 : 0) << 2) |
		((bIsAlertedTimerSet ? 1 : 0) << 3) |
		((bIsNumberOfSightConesThePlayerIsInSet ? 1 : 0) << 4) |
		((bIsAgentSet ? 1 : 0) << 5) |
		((bIsAgentLocationSet ? 1 : 0) << 6) |
		((bIsAgentSpawnLocationSet ? 1 : 0) << 7) |
		((bIsAgentSpawnRotationSet ? 1 : 0) << 8) |
		((bIsAgentDistanceFromSpawnSet ? 1 : 0) << 9) |
		((bIsPatrolSplineSet ? 1 : 0) << 10) |
		((bIsPlayerInSightConeSet ? 1 : 0) << 11) |
		((bIsDistanceFromPlayerSet ? 1 : 0) << 12) |
		((bIsSensedLocationSet ? 1 : 0) << 13) |
		((bIsCoverLocationSet ? 1 : 0) << 14) |
		((bIsMoveToLocationSet ? 1 : 0) << 15) |
		((bIsSightEnabled ? 1 : 0) << 16) |
		((bIsHearingEnabled ? 1 : 0) << 17);

	Ar << BitFlags;

	bIsPlayerSet = (BitFlags & (1 << 0)) != 0;
	bIsPlayerLocationSet = (BitFlags & (1 << 1)) != 0;
	bIsCombatTimerSet = (BitFlags & (1 << 2)) != 0;
	bIsAlertedTimerSet = (BitFlags & (1 << 3)) != 0;
	bIsNumberOfSightConesThePlayerIsInSet = (BitFlags & (1 << 4)) != 0;
	bIsAgentSet = (BitFlags & (1 << 5)) != 0;
	bIsAgentLocationSet = (BitFlags & (1 << 6)) != 0;
	bIsAgentSpawnLocationSet = (BitFlags & (1 << 7)) != 0;
	bIsAgentSpawnRotationSet = (BitFlags & (1 << 8)) != 0;
	bIsAgentDistanceFromSpawnSet = (BitFlags & (1 << 9)) != 0;
	bIsPatrolSplineSet = (BitFlags & (1 << 10)) != 0;
	bIsPlayerInSightConeSet = (BitFlags & (1 << 11)) != 0;
	bIsDistanceFromPlayerSet = (BitFlags & (1 << 12)) != 0;
	bIsSensedLocationSet = (BitFlags & (1 << 13)) != 0;
	bIsCoverLocationSet = (BitFlags & (1 << 14)) != 0;
	bIsMoveToLocationSet = (BitFlags & (1 << 15)) != 0;
	bIsSightEnabled = (BitFlags & (1 << 16)) != 0;
	bIsHearingEnabled = (BitFlags & (1 << 17)) != 0;
}

#endif