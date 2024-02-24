// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateInterface.h"
#include "UObject/Object.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "Templates/Function.h"
#include "FTransition.generated.h"

DECLARE_DELEGATE_RetVal(bool, FOnCheckConditionSignature)

UCLASS()
class CYBERPUNKDEMO_API UFTransition : public UObject
{
	GENERATED_BODY()

public:

	UFTransition();
	void Init(TObjectPtr<UStateInterface> toState);
	TObjectPtr<UStateInterface> ToState;

public:

	bool CheckTransition();
	FOnCheckConditionSignature OnCheckConditionDelegate;
	//TFunction<bool> Condition;
};
