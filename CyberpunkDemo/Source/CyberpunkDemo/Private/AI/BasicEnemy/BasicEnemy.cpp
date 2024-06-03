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
}

void ABasicEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	TArray<AActor*> overlappedActors;
	GetCapsuleComponent()->GetOverlappingActors(overlappedActors);
	if(GEngine)
	{
		for (int i = 0; i < overlappedActors.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, overlappedActors[i]->GetName());
		}
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
