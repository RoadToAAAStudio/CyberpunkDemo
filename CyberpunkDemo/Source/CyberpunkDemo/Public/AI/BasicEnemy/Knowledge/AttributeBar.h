// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFilledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmptiedSignature);

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERPUNKDEMO_API UAttributeBar : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnFilledSignature OnBarFilledDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEmptiedSignature OnBarEmptiedDelegate;

	void Add(float Amount);
	void Remove(float Amount);
	FORCEINLINE bool IsFull() const;
	FORCEINLINE float GetValue() const;
	FORCEINLINE void Fill();
	FORCEINLINE void Reset();
	
private:
	float Value = 0;
	bool bIsFull = false;
};
