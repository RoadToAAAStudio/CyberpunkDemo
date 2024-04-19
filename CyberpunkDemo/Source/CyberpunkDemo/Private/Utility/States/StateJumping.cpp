// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateJumping.h"
#include "MainCharacter/MainCharacter.h"

void UStateJumping::EnterState()
{
	
	Super::EnterState();
	bHasJumped = false;

	if (Owner->GetLastMovementState() == ECustomMovementState::Jumping)
	{
		Owner->JumpZVelocity = Owner->SecondJumpForce;
	}
	else
	{
		Owner->JumpZVelocity = Owner->JumpForce;
	}
}

void UStateJumping::ExitState()
{
	Super::ExitState();
	if (GravityTimer.IsValid()) Owner->GetWorld()->GetTimerManager().ClearTimer(GravityTimer);
	Owner->GravityScale = Owner->CustomGravity;
	Owner->MainCharacter->StopJumping();
}

void UStateJumping::Tick()
{
	Super::Tick();
	if (!Owner->MainCharacter->bIsCrouched && !bHasJumped)
	{
		bHasJumped = true;
		
		Owner->MainCharacter->Jump();
		Owner->bWantsToJump = false;

		if (Owner->GetLastMovementState() == ECustomMovementState::Crouching)
		{
			Owner->bWantsToCrouchCustom = true;
		}
	}

	if (Owner->Velocity.Z < 0 && !GravityTimer.IsValid() && Owner->GetLastMovementState() != ECustomMovementState::Jumping)
	{
		Owner->GravityScale = 0;
		Owner->GetWorld()->GetTimerManager().SetTimer(GravityTimer, FTimerDelegate::CreateLambda([this] { Owner->GravityScale = Owner->CustomGravity; }), 0.05f, false);
	}
	else if (Owner->Velocity.Z < 0 && !GravityTimer.IsValid() && Owner->GetLastMovementState() == ECustomMovementState::Jumping)
	{
		Owner->GravityScale = 1;
		Owner->GetWorld()->GetTimerManager().SetTimer(GravityTimer, this, &UStateJumping::SecondJumpGravity, 0.15f, false);
	}
	else if (Owner->Velocity.Z >= 0)
	{
		if (GravityTimer.IsValid()) Owner->GetWorld()->GetTimerManager().ClearTimer(GravityTimer);
		Owner->GravityScale = Owner->CustomGravity;
	}
}

void UStateJumping::SecondJumpGravity()
{
	Owner->GravityScale = 0;
	Owner->GetWorld()->GetTimerManager().SetTimer(GravityTimer, FTimerDelegate::CreateLambda([this] { Owner->GravityScale = Owner->CustomGravity; }), 0.15f, false);
}
