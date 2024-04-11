// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateJumping.h"

#include "Kismet/KismetStringLibrary.h"
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

	if (Owner->GetLastMovementState() == ECustomMovementState::Jumping)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "ENTERING DOUBLE JUMP");
		Owner->JumpZVelocity = Owner->SecondJumpForce;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Silver, "ENTERING JUMP");
		Owner->JumpZVelocity = Owner->JumpForce;
	}
}

void UStateJumping::ExitState()
{
	Super::ExitState();
	if (GravityTimer.IsValid()) Owner->GetWorld()->GetTimerManager().ClearTimer(GravityTimer);
	Owner->GravityScale = Owner->CustomGravity;
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "EXITING JUMP");
	Owner->SetLastMovementState(ECustomMovementState::Jumping);
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

	if (Owner->Velocity.Z < 0 && !GravityTimer.IsValid())
	{
		Owner->GravityScale = 0;
		Owner->GetWorld()->GetTimerManager().SetTimer(GravityTimer, FTimerDelegate::CreateLambda([this] { Owner->GravityScale = Owner->CustomGravity * 2; }), 0.05f, false);
	}
	else if (Owner->Velocity.Z >= 0)
	{
		if (GravityTimer.IsValid()) Owner->GetWorld()->GetTimerManager().ClearTimer(GravityTimer);
		Owner->GravityScale = Owner->CustomGravity;
	}
}
