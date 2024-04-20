// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "State.generated.h"

class UTransition;
// Abstract state class from which all the states will derive
UCLASS(Abstract)
class CYBERPUNKDEMO_API UState : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UTransition>> Transitions;
	
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
