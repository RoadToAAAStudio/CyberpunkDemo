// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBarFilledSignature);

UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERPUNKDEMO_API UAttributeBar : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0;

	UPROPERTY(BlueprintAssignable)
	FOnBarFilledSignature OnBarFilledDelegate;
	
public:	
	// Sets default values for this component's properties
	UAttributeBar();

	// Add an amount to CurrentValue
	void AddAmount(float Amount);

	// Remove an amount to CurrentValue
	void RemoveAmount(float Amount);

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
