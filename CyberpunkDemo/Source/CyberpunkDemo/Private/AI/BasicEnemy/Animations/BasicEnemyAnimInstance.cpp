// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BasicEnemy/Animations/BasicEnemyAnimInstance.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBasicEnemyAnimInstance::NativeInitializeAnimation()
{
	if (BasicEnemy = Cast<ABasicEnemy>(TryGetPawnOwner()))
	{
		CharacterMovementComponent = BasicEnemy->GetCharacterMovement();
	}
}

void UBasicEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
		bIsCrouching = CharacterMovementComponent->bWantsToCrouch;
	}
}
