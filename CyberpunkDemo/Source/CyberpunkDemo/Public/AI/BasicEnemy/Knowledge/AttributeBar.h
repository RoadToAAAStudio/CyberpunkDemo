// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFilledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmptiedSignature);

/*
 * Represent a bar that can be filled
 * It has delegates for when it is filled or emptied
 */

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERPUNKDEMO_API UAttributeBar : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnFilledSignature OnBarFilledDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEmptiedSignature OnBarEmptiedDelegate;
	
private:
	float Value = 0;
	bool bIsFull = false;
	
public:
	void Add(float Amount);
	void Remove(float Amount);
	bool IsFull() const;
	float GetValue() const;
	void Fill();
	void Reset();
};
