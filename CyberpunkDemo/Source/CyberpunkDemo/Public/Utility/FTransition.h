// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FState.h"
#include "UObject/Object.h"
#include "Templates/Function.h"
#include "FTransition.generated.h"

DECLARE_DELEGATE_RetVal(bool, FOnCheckConditionSignature)

UCLASS()
class CYBERPUNKDEMO_API UFTransition : public UObject
{
	GENERATED_BODY()

public:

	// Basic constructor
	UFTransition();

	// Init function that substitutes the constructor
	void Init(const TObjectPtr<UFState> toState);

	// The state this transition points to
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFState> ToState;

public:
	
	bool CheckTransition();

	FOnCheckConditionSignature OnCheckConditionDelegate;

	TFunction<bool()> Condition = nullptr;
};
