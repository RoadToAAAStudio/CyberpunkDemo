// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "UObject/Object.h"
#include "Templates/Function.h"
#include "Transition.generated.h"

DECLARE_DELEGATE_RetVal(bool, FOnCheckConditionSignature)

UCLASS()
class CYBERPUNKDEMO_API UTransition : public UObject
{
	GENERATED_BODY()

public:

	// Basic constructor
	UTransition();

	// Init function that substitutes the constructor
	void Init(const TObjectPtr<UState> toState);

	// The state this transition points to
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UState> ToState;
	
	bool CheckTransition();

	FOnCheckConditionSignature OnCheckConditionDelegate;
};
