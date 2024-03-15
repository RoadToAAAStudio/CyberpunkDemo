// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Interface.h"
#include "Hackerable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHackerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYBERPUNKDEMO_API IHackerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual TSet<TSubclassOf<UGameplayAbility>> GetPossibleHacks() const;

	virtual void Highlight() const;
};
