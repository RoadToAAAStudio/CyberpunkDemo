// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter/CustomCharacterMovementComponent.h"
#include "Utility/FState.h"
#include "StateVault.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateVault : public UFState
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCustomCharacterMovementComponent> Owner;

	FVector Velocity;

public:

	void SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner);
	
	void EnterState() override;
	void ExitState() override;
	void Tick() override;
};
