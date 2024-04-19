// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FState.h"
#include "MainCharacter/CustomCharacterMovementComponent.h"
#include "StateCyberpunkProject.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateCyberpunkProject : public UFState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY() TObjectPtr<UCustomCharacterMovementComponent> Owner;
	ECustomMovementState State;

public:

	void Initialize(TObjectPtr<UCustomCharacterMovementComponent> owner, ECustomMovementState state);

	virtual void EnterState() override;
	virtual void Tick() override;
	virtual void ExitState() override;
};
