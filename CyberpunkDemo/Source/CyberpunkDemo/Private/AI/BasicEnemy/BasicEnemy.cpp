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
	PrimaryActorTick.bCanEverTick = false;
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