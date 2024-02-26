// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FState.generated.h"

class UFTransition;
// Abstract state class from which all the state will derive
UCLASS(Abstract)
class CYBERPUNKDEMO_API UFState : public UObject
{
	GENERATED_BODY()

public:

	TArray<TObjectPtr<UFTransition>> Transitions;
	
	// Called when entering the state
	virtual void EnterState();

	// Called when the state machine is in this state
	virtual void Tick();

	// Called before exiting this state
	virtual void ExitState();

	// In each substate then we will save a reference to the owner of the state machine, its type DEPENDS on the project
	// UPROPERTY
	// TObjectPtr<TYPE> Owner;
	//
	// void SetOwner(TObjectPtr<TYPE> owner);
};
