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
}

void UStateJumping::ExitState()
{
	Super::ExitState();
	Owner->SetLastMovementState(ECustomMovementState::Jumping);
	Owner->MainCharacter->StopJumping();
	Owner->GravityScale = Owner->CustomGravity;
	GravityTimer.Invalidate();
}

void UStateJumping::Tick()
{
	Super::Tick();
	if (!Owner->MainCharacter->bIsCrouched && !bHasJumped)
	{
		bHasJumped = true;
		if (!Owner->IsMovingOnGround())
		{
			Owner->JumpZVelocity = Owner->SecondJumpForce;
		}
		else
		{
			Owner->JumpZVelocity = Owner->JumpForce;
		}

		//Owner->GravityScale = 0;
		Owner->MainCharacter->Jump();
		Owner->bWantsToJump = false;

		if (Owner->GetLastMovementState() == ECustomMovementState::Crouching)
		{
			Owner->bWantsToCrouchCustom = true;
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString(Owner->Velocity.ToString()));
		
	if (Owner->Velocity.Z < 0 && !GravityTimer.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Blue, "REACHED JUMP APEX");
		Owner->GravityScale = 0;
		Owner->GetWorld()->GetTimerManager().SetTimer(GravityTimer, FTimerDelegate::CreateLambda([this] { Owner->GravityScale = Owner->CustomGravity * 2; }), 0.05f, false);
	}
}
