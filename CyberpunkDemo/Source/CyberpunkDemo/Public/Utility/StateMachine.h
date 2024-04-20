// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Transition.h"
#include "UObject/Object.h"
#include "StateMachine.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UState> CurrentState;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UState>> States;
	
	
	UStateMachine();

	void Init(const TObjectPtr<UState> StartingState);

	void AddState(const TObjectPtr<UState> NewState);

	void Tick();
};
