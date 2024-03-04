// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateJumping.h"

#include "MainCharacter/MainCharacter.h"

void UStateJumping::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateJumping::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Jumping);
	bHasJumped = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "I AM JUMPING");
}

void UStateJumping::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "I AM NOT JUMPING");
	Owner->MainCharacter->StopJumping();
}

void UStateJumping::Tick()
{
	Super::Tick();
	if (!Owner->MainCharacter->bIsCrouched && !bHasJumped)
	{
		if (!Owner->IsMovingOnGround())
		{
			Owner->JumpZVelocity = Owner->SecondJumpForce;
		}
		else
		{
			Owner->JumpZVelocity = Owner->JumpForce;
		}

		Owner->MainCharacter->Jump();
		bHasJumped = true;
		Owner->bWantsToJump = false;
	}
}
