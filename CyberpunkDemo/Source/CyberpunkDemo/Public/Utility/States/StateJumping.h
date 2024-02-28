// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/FState.h"
#include "MainCharacter/CustomCharacterMovementComponent.h"
#include "StateJumping.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateJumping : public UFState
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCustomCharacterMovementComponent> Owner;

	bool bHasJumped;

public:

	void SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner);
	
	void EnterState() override;
	void ExitState() override;
	void Tick() override;
};
