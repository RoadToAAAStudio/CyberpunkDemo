// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBarFilledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBarEmptiedSignature);

/*
 * Represent a bar that can be filled
 * It has delegates for when it is filled or emptied
 */

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERPUNKDEMO_API UAttributeBar : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFull = false;

	UPROPERTY(BlueprintAssignable)
	FOnBarFilledSignature OnBarFilledDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBarEmptiedSignature OnBarEmptiedDelegate;
	
public:	
	// Sets default values for this component's properties
	UAttributeBar();

	// Add an amount to currentValue
	void AddAmount(float Amount);

	// Remove an amount to currentValue
	void RemoveAmount(float Amount);

	// Set current value to max
	void Fill();
	
	// Reset current value
	void Reset();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
