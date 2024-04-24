// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"


void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
	
	if (UCustomCharacterMovementComponent* CCMC = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0))->GetCustomCharacterMovementComponent())
	{
		CCMC->OnEnterCustomMovementState.AddUniqueDynamic(this, &ACustomPlayerController::PlayCameraShake);
		CCMC->OnExitCustomMovementState.AddUniqueDynamic(this, &ACustomPlayerController::StopCameraShake);
	}
}

void ACustomPlayerController::PlayCameraShake(ECustomMovementState State)
{
	switch (State)
	{
	case ECustomMovementState::Idle:
		ClientStartCameraShake(IdleCameraShake);
		break;

	case ECustomMovementState::Walking:
		ClientStartCameraShake(WalkCameraShake);
		break;

	case ECustomMovementState::Running:
		ClientStartCameraShake(RunCameraShake);
		break;
	}
}

void ACustomPlayerController::StopCameraShake(ECustomMovementState State)
{
	switch (State)
	{
	case ECustomMovementState::Idle:
		ClientStopCameraShake(IdleCameraShake);
		break;

	case ECustomMovementState::Walking:
		ClientStopCameraShake(WalkCameraShake);
		break;

	case ECustomMovementState::Running:
		ClientStopCameraShake(RunCameraShake);
		break;
	}
}



