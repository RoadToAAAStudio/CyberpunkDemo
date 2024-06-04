// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"

ABasicEnemy::ABasicEnemy()
{
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
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

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		this->AIZone = Cast<AAIZone>(OtherActor);
	}
}
#pragma endregion 
