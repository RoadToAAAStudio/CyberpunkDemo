// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "FTransition.h"
#include "UObject/Object.h"
#include "StateMachine.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UFState> CurrentState;
	TArray<TObjectPtr<UFState>> States;
	TMap<TObjectPtr<UFState>, TArray<TObjectPtr<UFTransition>>> Transitions;
	
protected:

private:

public:
	UStateMachine();

	void Init(const TObjectPtr<UFState> StartingState);

	void AddState(const TObjectPtr<UFState> NewState);

	void AddTransition(const TObjectPtr<UFState> FromState, const TObjectPtr<UFTransition> NewTransition);
	
	void Tick();
};
