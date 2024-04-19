// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateCyberpunkProject.h"
#include "UStateMantle.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UUStateMantle : public UStateCyberpunkProject
{
	GENERATED_BODY()

public:

	void EnterState() override;
	void ExitState() override;
	void Tick() override;
};
