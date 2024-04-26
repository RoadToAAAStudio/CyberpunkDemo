// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateCyberpunkProject.h"
#include "StateDash.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UStateDash : public UStateCyberpunkProject
{
	GENERATED_BODY()

	FTimerHandle DashTimer;
	
public:

	void EnterState() override;
	void ExitState() override;
	void Tick() override;
};
