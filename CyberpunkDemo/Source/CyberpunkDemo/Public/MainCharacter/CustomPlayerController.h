// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacter/CustomCharacterMovementComponent.h"
#include "CustomPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class CYBERPUNKDEMO_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UCameraShakeBase> IdleCameraShake;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UCameraShakeBase> WalkCameraShake;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UCameraShakeBase> RunCameraShake;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UCameraShakeBase> DashCameraShake;

protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	FTimerHandle timer;
	
	// Begin Actor interface
	virtual void BeginPlay() override;
	// End Actor interface

	UFUNCTION()
	void PlayCameraShake(ECustomMovementState State);

	UFUNCTION()
	void StopCameraShake(ECustomMovementState State);
};
