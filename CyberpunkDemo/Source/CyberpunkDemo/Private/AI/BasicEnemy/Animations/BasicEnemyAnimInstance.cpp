// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BasicEnemy/Animations/BasicEnemyAnimInstance.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AITypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UBasicEnemyAnimInstance::NativeInitializeAnimation()
{
	if (BasicEnemy = Cast<ABasicEnemy>(TryGetPawnOwner()))
	{
		CharacterMovementComponent = BasicEnemy->GetCharacterMovement();
	}
}

void UBasicEnemyAnimInstance::NativeBeginPlay()
{
	if (BasicEnemy)
	{
		BasicEnemyController = BasicEnemy->BasicEnemyController;
	}
}

void UBasicEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (CharacterMovementComponent)
	{
		VelocityDirection = CharacterMovementComponent->Velocity;
		VelocityDirection.Normalize();

		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);

		if (BasicEnemyController)
		{
			bHasFocalPoint = BasicEnemyController->GetFocalPoint() != FAISystem::InvalidLocation;
			if (bHasFocalPoint)
			{
				FTransform LookAtTransform = UKismetAnimationLibrary::K2_LookAt(BasicEnemy->GetActorTransform(), BasicEnemyController->GetFocalPoint(), FVector(1.0f, 0.0f, 0.0f), false, FVector::Zero(), 180.0f);
				RelativeRotationToFocalPoint = UKismetAnimationLibrary::CalculateDirection(VelocityDirection, LookAtTransform.Rotator());
			}
		}
	}
}