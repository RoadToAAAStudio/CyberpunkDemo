// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "CyberpunkDemo/DebugMacros.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "AI/Utility/SplineContainer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSingleNodeInstance.h"

ABasicEnemy::ABasicEnemy()
{
	BehaviourToTest = EBasicEnemyBehaviour::None;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick = false;
#endif
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

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);

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
		const EBasicEnemyState State = KnowledgeComponent->PersonalKnowledge.AgentState;
		const EBasicEnemyBehaviour Behaviour = KnowledgeComponent->PersonalKnowledge.AgentBehaviour;
		const FString String = FString::Printf(TEXT("%s\nState: %s\nBehaviour: %s"), *GetActorNameOrLabel(), *(UEnum::GetDisplayValueAsText(State).ToString()), *(UEnum::GetDisplayValueAsText(Behaviour).ToString()));

		DRAW_STRING(this, GetActorLocation() + FVector::UpVector * 200.0f, String, FColor::White, 0.0f);
	}

	// Debug Name
	{
		const FString String = FString::Printf(TEXT("AgentName: %s----------------------------------------------"), *GetActorNameOrLabel());
		PRINT_ON_SCREEN(0.0f, FColor::Yellow, String, false);
	}

	// Debug Sensors
	{
		const FString String = FString::Printf(TEXT("[SENSORS]\n"
			"Sight (%s): %.2f - Hearing (%s): %.2f"), KnowledgeComponent->IsSightEnabled()? "*" : "-", KnowledgeComponent->GetSightBarValue(), KnowledgeComponent->IsHearingEnabled() ? "*" : "-", KnowledgeComponent->GetHearingBarValue());
		PRINT_ON_SCREEN(0.0f, FColor(0, 140, 255, 255), String, false);
	}

	// Debug Personal Knowledge
	{
		FString String = FString::Printf(TEXT("[PERSONAL KNOWLEDGE]\n"));
	
		const FSettablePawn& Agent = KnowledgeComponent->PersonalKnowledge.Agent;
		const FString AgentName = Agent.Get() ? Agent.Get()->GetActorNameOrLabel() : TEXT("null");
		const FSettableVector& AgentLocation = KnowledgeComponent->PersonalKnowledge.AgentLocation;
		String += FString::Printf(TEXT("Agent (%s): %s - Location (%s): (%.2f, %.2f, %.2f)\n"), Agent.IsSet() ? "*" : "-", *AgentName, AgentLocation.IsSet() ? "*" : "-", AgentLocation.Get().X, AgentLocation.Get().Y, AgentLocation.Get().Z);
		
		const FSettableVector& AgentSpawnLocation = KnowledgeComponent->PersonalKnowledge.AgentSpawnLocation;
		const FSettableRotator& AgentSpawnRotation = KnowledgeComponent->PersonalKnowledge.AgentSpawnRotation;
		const FSettableFloat& AgentDistanceFromSpawn = KnowledgeComponent->PersonalKnowledge.AgentDistanceFromSpawn;
		String += FString::Printf(TEXT("(Spawn) Location (%s): (%.2f, %.2f, %.2f) - Rotation (%s): (%.2f, %.2f, %.2f) - Distance (%s) %.2f\n"), AgentSpawnLocation.IsSet() ? "*" : "-", AgentSpawnLocation.Get().X, AgentSpawnLocation.Get().Y, AgentSpawnLocation.Get().Z, AgentSpawnRotation.IsSet() ? "*" : "-", AgentSpawnRotation.Get().Roll, AgentSpawnRotation.Get().Pitch, AgentSpawnRotation.Get().Yaw, AgentDistanceFromSpawn.IsSet() ? "*" : "-", AgentDistanceFromSpawn.Get());
		
		const EBasicEnemyState State = KnowledgeComponent->PersonalKnowledge.AgentState;
		const EBasicEnemyBehaviour Behaviour = KnowledgeComponent->PersonalKnowledge.AgentBehaviour;
		String += FString::Printf(TEXT("State: %s - Behaviour: %s\n"), *(UEnum::GetDisplayValueAsText(State).ToString()), *(UEnum::GetDisplayValueAsText(Behaviour).ToString()));
		
		const FSettableSpline& SettablePatrolSpline = KnowledgeComponent->PersonalKnowledge.PatrolSpline;
		const ASplineContainer* PatrolSplineContainer = SettablePatrolSpline.Get();
		const FString PatrolSplineName = PatrolSplineContainer ? PatrolSplineContainer->Spline.GetName() : TEXT("null");
		String += FString::Printf(TEXT("PatrolSpline (%s): %s\n"), SettablePatrolSpline.IsSet() ? "*" : "-", *(PatrolSplineName));

		const TArray<FVector>& PatrolWaypoints = KnowledgeComponent->PersonalKnowledge.PatrolWaypoints;
		for (int i = 0; i < PatrolWaypoints.Num(); i++)
		{
			const FVector PatrolWaypoint = PatrolWaypoints[i];
			String += FString::Printf(TEXT(" - Waypoint (%d): (%.2f, %.2f, %.2f)\n"), (i + 1), PatrolWaypoint.X, PatrolWaypoint.Y, PatrolWaypoint.Z);
		}

		const FSettableMainCharacter& PlayerInCone = KnowledgeComponent->PersonalKnowledge.PlayerInSightCone;
		const bool bIsPlayerInSightCone = PlayerInCone.Get() != nullptr;
		const FSettableFloat& DistanceFromPlayerInCone = KnowledgeComponent->PersonalKnowledge.DistanceFromPlayer;
		String += FString::Printf(TEXT("PlayerInSightCone (%s): %s - Distance (%s): %.2f\n"), PlayerInCone.IsSet() ? "*" : "-", bIsPlayerInSightCone ? TEXT("true") : TEXT("false"), DistanceFromPlayerInCone.IsSet() ? "*" : "-", DistanceFromPlayerInCone.Get());

		const FSettableVector& SensedLocation = KnowledgeComponent->PersonalKnowledge.SensedLocation;
		String += FString::Printf(TEXT("SensedLocation (%s): (%.2f, %.2f, %.2f)\n"), SensedLocation.IsSet() ? "*" : "-", SensedLocation.Get().X, SensedLocation.Get().Y, SensedLocation.Get().Z);

		const FSettableVector& CoverLocation = KnowledgeComponent->PersonalKnowledge.CoverLocation;
		String += FString::Printf(TEXT("CoverLocation (%s): (%.2f, %.2f, %.2f)\n"), CoverLocation.IsSet() ? "*" : "-", CoverLocation.Get().X, CoverLocation.Get().Y, CoverLocation.Get().Z);

		const FSettableVector& MoveToLocation = KnowledgeComponent->PersonalKnowledge.MoveToLocation;
		String += FString::Printf(TEXT("MoveToLocation (%s): (%.2f, %.2f, %.2f)"), MoveToLocation.IsSet() ? "*" : "-", MoveToLocation.Get().X, MoveToLocation.Get().Y, MoveToLocation.Get().Z);

		PRINT_ON_SCREEN(0.0f, FColor(0, 140, 255, 255), String, false);
	}
}

void ABasicEnemy::DebugBehaviours() const
{
	if (!BasicEnemyController) return;

	const UBasicEnemyKnowledgeComponent* KnowledgeComponent = BasicEnemyController->KnowledgeComponent;
	if (!KnowledgeComponent) return;

	const EBasicEnemyBehaviour Behaviour = KnowledgeComponent->PersonalKnowledge.AgentBehaviour;
	switch (Behaviour)
	{
	case EBasicEnemyBehaviour::Idle:
	{
		const USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		if (SkeletalMeshComponent->GetAnimationMode() == EAnimationMode::AnimationSingleNode)
		{
			if (UAnimSingleNodeInstance* Animation = Cast<class UAnimSingleNodeInstance>(SkeletalMeshComponent->GetAnimInstance()))
			{
				if (UAnimationAsset* AnimationAsset = Animation->GetAnimationAsset())
				{
					const FString String = FString::Printf(TEXT("Performing: %s\n"), *AnimationAsset->GetName());

					DRAW_STRING(this, GetActorLocation() + FVector::UpVector * 150.0f, String, FColor::Yellow, 0.0f);
				}
			}
		}	
		break;
	}
	case EBasicEnemyBehaviour::ReturnToSpawnPoint:
	{
		const FVector& SpawnLocation = KnowledgeComponent->PersonalKnowledge.AgentSpawnLocation.Get();
		const float DistanceFromSpawn = KnowledgeComponent->PersonalKnowledge.AgentDistanceFromSpawn.Get();
		const FString DistanceFromSpawnString = FString::Printf(TEXT("Distance: %.2f"), DistanceFromSpawn);
		DRAW_SPHERE(this, SpawnLocation, 25, 12, FColor::Green, 0.0f);
		DRAW_LINE(this, GetActorLocation(), SpawnLocation, FColor::Yellow, 0.0f);
		DRAW_STRING(this, SpawnLocation + FVector::UpVector * 50.0f, DistanceFromSpawnString, FColor::White, 0.0f);
		break;
	}
	case EBasicEnemyBehaviour::Patrol:
	{
		const USplineComponent* Spline = KnowledgeComponent->PersonalKnowledge.PatrolSpline.Get()->Spline;
		const FVector& FirstSplinePointLocation = KnowledgeComponent->PersonalKnowledge.PatrolWaypoints[0];
		const FVector& LastSplinePointLocation = KnowledgeComponent->PersonalKnowledge.PatrolWaypoints[KnowledgeComponent->PersonalKnowledge.PatrolWaypoints.Num() - 1];
		DRAW_STRING(this, FirstSplinePointLocation + FVector::UpVector * 50.0f, TEXT("Spline First Point"), FColor::White, 0.0f);
		DRAW_STRING(this, LastSplinePointLocation + FVector::UpVector * 50.0f, TEXT("Spline Last Point"), FColor::White, 0.0f);
		break;
	}
	case EBasicEnemyBehaviour::BlindInvestigation:
	{
		break;
	}
	case EBasicEnemyBehaviour::Investigation:
	{
		break;
	}
	case EBasicEnemyBehaviour::MoveToBetterPosition:
	{
		break;
	}
	case EBasicEnemyBehaviour::QuickMeleeAttack:
	{
		break;
	}
	case EBasicEnemyBehaviour::ThrowGrenade:
	{
		break;
	}
	case EBasicEnemyBehaviour::MoveToCover:
	{
		break;
	}
	case EBasicEnemyBehaviour::ShootFromCover:
	{
		break;
	}
	case EBasicEnemyBehaviour::ThrowGrenadeFromCover:
	{
		break;
	}
	}
}

void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		AIZone = Cast<AAIZone>(OtherActor);
	}
}