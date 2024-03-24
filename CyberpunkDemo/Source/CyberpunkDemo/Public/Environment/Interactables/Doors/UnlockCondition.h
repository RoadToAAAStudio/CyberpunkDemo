// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UnlockCondition.generated.h"

enum class EInspectInput:uint8;

UCLASS(Abstract)
class CYBERPUNKDEMO_API UUnlockCondition : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UUnlockCondition();

	virtual void AddUnlockInput(TArray<EInspectInput>& ArrayInputs);
	
	virtual bool Check();
};
