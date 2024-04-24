// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UBehaviour : public UObject
{
	GENERATED_BODY()
	
public:
	// Can this behaviour be started i.e. is it a valid option for selection?
	bool IsValidSelectionOption() const;

	// Is the behaviour currently cooling down?
	bool IsCoolingDown() const;

	// Are the condition valid for this behaviour to be started
	virtual bool AreStartingConditionMet() const;

	// Calculates a score used for behaviour selection when we have multiple options
	virtual float GetSelectionScore() const;
};
