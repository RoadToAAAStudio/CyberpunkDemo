// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateVault.h"

#include "MainCharacter/MainCharacter.h"

void UStateVault::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateVault::EnterState()
{
	Super::EnterState();
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "ENTERING VAULT");
	Owner->SetCurrentMovementState(ECustomMovementState::Vaulting);
	Owner->MainCharacter->DisableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
	Owner->bWantsToJump = false;
	Owner->MainCharacter->Vault();
}

void UStateVault::ExitState()
{
	Super::ExitState();
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "EXITING VAULT");
	Owner->SetLastMovementState(ECustomMovementState::Vaulting);
	Owner->MainCharacter->EnableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
}

void UStateVault::Tick()
{
	Super::Tick();
}
