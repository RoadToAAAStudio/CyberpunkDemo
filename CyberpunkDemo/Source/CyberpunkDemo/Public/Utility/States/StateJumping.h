// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateCyberpunkProject.h"
#include "StateJumping.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateJumping : public UStateCyberpunkProject
{
	GENERATED_BODY()

	bool bHasJumped;
	bool bIsFalling;

	FTimerHandle GravityTimer;

public:

	void EnterState() override;
	void ExitState() override;
	void Tick() override;

	void SecondJumpGravity();
};
