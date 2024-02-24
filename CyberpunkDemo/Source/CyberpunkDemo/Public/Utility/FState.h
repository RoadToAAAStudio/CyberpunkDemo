// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FState.generated.h"

// Abstract state class from which all the state will derive
UCLASS(Abstract)
class CYBERPUNKDEMO_API UFState : public UObject
{
	GENERATED_BODY()

public:

	// Called when entering the state
	virtual void EnterState();

	// Called when the state machine is in this state
	virtual void Tick();

	// Called before exiting this state
	virtual void ExitState();
};
