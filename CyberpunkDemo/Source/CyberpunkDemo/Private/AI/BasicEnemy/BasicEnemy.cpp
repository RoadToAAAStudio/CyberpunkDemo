// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"

ABasicEnemy::ABasicEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABasicEnemy::BeginPlay()
{
	BasicEnemyController = Cast<ABasicEnemyController>(GetController());

	Super::BeginPlay();
}