// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateCyberpunkProject.h"
#include "StateVault.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateVault : public UStateCyberpunkProject
{
	GENERATED_BODY()

	FVector Velocity;

public:

	void EnterState() override;
	void ExitState() override;
	void Tick() override;
};
