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
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UFState> CurrentState;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UFState>> States;
	
	
	UStateMachine();

	void Init(const TObjectPtr<UFState> StartingState);

	void AddState(const TObjectPtr<UFState> NewState);

	void Tick();
};
