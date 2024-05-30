// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		this->AIZone = Cast<AAIZone>(OtherActor);
	}

	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
	if (BasicEnemyController)
	{
		BasicEnemyController->Initialize(this);
	}
}
#pragma endregion 
