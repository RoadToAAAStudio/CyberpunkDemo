// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "CyberpunkDemo/DebugMacros.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "AI/Utility/SplineContainer.h"

ABasicEnemy::ABasicEnemy()
{
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
}

void ABasicEnemy::Tick(float DeltaSeconds)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bDebugKnowledge)
	{
		DebugKnowledge();
	}
	if (bDebugBehaviours)
	{
		DebugBehaviours();
	}
#endif
}

void ABasicEnemy::RegisterAIZone(AAIZone* NewAIZone)
{
	if (NewAIZone == nullptr) return;
	AIZone = NewAIZone;

	if (Cast<ABasicEnemyController>(GetController()))
	{
		Cast<ABasicEnemyController>(GetController())->RegisterAIZone(NewAIZone);
	}
}

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
	if (BasicEnemyController)
	{
		BasicEnemyController->Initialize(this);
	}
}

void ABasicEnemy::DebugKnowledge() const
{
	if (!BasicEnemyController) return;

	const UBasicEnemyKnowledgeComponent* KnowledgeComponent = BasicEnemyController->KnowledgeComponent;
	if (!KnowledgeComponent) return;

	
	// Debug General AI
	{
		const EBasicEnemyState state = KnowledgeComponent->PersonalKnowledge.AgentState;
		const EBasicEnemyBehaviour behaviour = KnowledgeComponent->PersonalKnowledge.AgentBehaviour;
		const FString string = FString::Printf(TEXT("%s\nState: %s\nBehaviour: %s"), *GetActorNameOrLabel(), *(UEnum::GetDisplayValueAsText(state).ToString()), *(UEnum::GetDisplayValueAsText(behaviour).ToString()));

		DRAW_STRING(GetActorLocation() + FVector::UpVector * 200.0f, string, FColor::White, 0.0f);	
	}

	// Debug Sensors
	{
		const FString string = FString::Printf(TEXT("AgentName: %s----------------------------------"), *GetActorNameOrLabel());
		PRINT_ON_SCREEN(0.0f, FColor::Yellow, string, false);
	}

	// Debug Name
	{
		const FString string = FString::Printf(TEXT("[SENSORS]\n"
			"Sight (%s): %.2f - Hearing (%s): %.2f"), KnowledgeComponent->IsSightEnabled()? "*" : "-", KnowledgeComponent->GetSightBarValue(), KnowledgeComponent->IsHearingEnabled() ? "*" : "-", KnowledgeComponent->GetHearingBarValue());
		PRINT_ON_SCREEN(0.0f, FColor(0, 140, 200, 255), string, false);
	}

	// Debug Personal Knowledge
	{
		FString string = FString::Printf(TEXT("[PERSONAL KNOWLEDGE]\n"));
	
		const FSettablePawn agent = KnowledgeComponent->PersonalKnowledge.Agent;
		const FString agentName = agent.Get() ? agent.Get()->GetActorNameOrLabel() : TEXT("null");
		const FSettableVector agentLocation = KnowledgeComponent->PersonalKnowledge.AgentLocation;
		string += FString::Printf(TEXT("Agent (%s): %s - Location (%s): (%.2f, %.2f, %.2f)\n"), agent.IsSet() ? "*" : "-", *agentName, agentLocation.IsSet() ? "*" : "-", agentLocation.Get().X, agentLocation.Get().Y, agentLocation.Get().Z);
		
		const FSettableVector agentSpawnLocation = KnowledgeComponent->PersonalKnowledge.AgentSpawnLocation;
		const FSettableRotator agentSpawnRotation = KnowledgeComponent->PersonalKnowledge.AgentSpawnRotation;
		const FSettableFloat agentDistanceFromSpawn = KnowledgeComponent->PersonalKnowledge.AgentDistanceFromSpawn;
		string += FString::Printf(TEXT("(Spawn) Location (%s): (%.2f, %.2f, %.2f) - Rotation (%s): (%.2f, %.2f, %.2f) - Distance (%s) %.2f\n"), agentSpawnLocation.IsSet() ? "*" : "-", agentSpawnLocation.Get().X, agentSpawnLocation.Get().Y, agentSpawnLocation.Get().Z, agentSpawnRotation.IsSet() ? "*" : "-", agentSpawnRotation.Get().Roll, agentSpawnRotation.Get().Pitch, agentSpawnRotation.Get().Yaw, agentDistanceFromSpawn.IsSet() ? "*" : "-", agentDistanceFromSpawn.Get());
		
		const EBasicEnemyState state = KnowledgeComponent->PersonalKnowledge.AgentState;
		const EBasicEnemyBehaviour behaviour = KnowledgeComponent->PersonalKnowledge.AgentBehaviour;
		string += FString::Printf(TEXT("State: %s - Behaviour: %s\n"), *(UEnum::GetDisplayValueAsText(state).ToString()), *(UEnum::GetDisplayValueAsText(behaviour).ToString()));
		
		const TArray<FVector>& patrolWaypoints = KnowledgeComponent->PersonalKnowledge.PatrolWaypoints;
		const FSettableSpline patrolSpline = KnowledgeComponent->PersonalKnowledge.PatrolSpline;
		const ASplineContainer* patrolSplineContainer = patrolSpline.Get();
		const FString patrolSplineName = patrolSplineContainer ? patrolSplineContainer->Spline.GetName() : TEXT("null");
		string += FString::Printf(TEXT("PatrolSpline (%s): %s\n"), patrolSpline.IsSet() ? "*" : "-", *(patrolSplineName));
		for (int i = 0; i < patrolWaypoints.Num(); i++)
		{
			const FVector patrolWaypoint = patrolWaypoints[i];
			string += FString::Printf(TEXT(" - Waypoint (%d): (%.2f, %.2f, %.2f)\n"), (i + 1), patrolWaypoint.X, patrolWaypoint.Y, patrolWaypoint.Z);
		}

		const FSettableMainCharacter playerInCone = KnowledgeComponent->PersonalKnowledge.PlayerInSightCone;
		const bool bIsPlayerInSightCone = playerInCone.Get() != nullptr;
		const FSettableFloat distanceFromPlayerInCone = KnowledgeComponent->PersonalKnowledge.DistanceFromPlayer;
		string += FString::Printf(TEXT("PlayerInSightCone (%s): %s - Distance (%s): %.2f\n"), playerInCone.IsSet() ? "*" : "-", bIsPlayerInSightCone ? TEXT("true") : TEXT("false"), distanceFromPlayerInCone.IsSet() ? "*" : "-", distanceFromPlayerInCone.Get());

		const FSettableVector sensedLocation = KnowledgeComponent->PersonalKnowledge.HeardStimulusLocation;
		string += FString::Printf(TEXT("SensedLocation (%s): (%.2f, %.2f, %.2f)\n"), sensedLocation.IsSet() ? "*" : "-", sensedLocation.Get().X, sensedLocation.Get().Y, sensedLocation.Get().Z);

		const FSettableVector coverLocation = KnowledgeComponent->PersonalKnowledge.CoverLocation;
		string += FString::Printf(TEXT("CoverLocation (%s): (%.2f, %.2f, %.2f)\n"), coverLocation.IsSet() ? "*" : "-", coverLocation.Get().X, coverLocation.Get().Y, coverLocation.Get().Z);

		const FSettableVector moveToLocation = KnowledgeComponent->PersonalKnowledge.MoveToLocation;
		string += FString::Printf(TEXT("MoveToLocation (%s): (%.2f, %.2f, %.2f)\n"), moveToLocation.IsSet() ? "*" : "-", moveToLocation.Get().X, moveToLocation.Get().Y, moveToLocation.Get().Z);

		PRINT_ON_SCREEN(0.0f, FColor(0, 140, 200, 255), string, false);
	}
}

void ABasicEnemy::DebugBehaviours() const
{
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		this->AIZone = Cast<AAIZone>(OtherActor);
	}
}
#pragma endregion 
