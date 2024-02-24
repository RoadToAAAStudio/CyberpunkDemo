// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "StateInterface.h"
#include "FTransition.h"
#include "UObject/Object.h"
#include "StateMachine.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	TObjectPtr<UStateInterface> CurrentState;
	TArray<TObjectPtr<UStateInterface>> States;
	TMap<TObjectPtr<UStateInterface>, TArray<TObjectPtr<UFTransition>>> Transitions;
	
protected:

private:

public:
	UStateMachine();

	void AddState();

	void AddTransition();
	
	void Tick();

	
};
