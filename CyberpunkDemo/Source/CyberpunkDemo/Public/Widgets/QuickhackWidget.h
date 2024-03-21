// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "QuickhackWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UQuickhackWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Widget elements 
	UPROPERTY(meta = (BindWidget))
	UButton* HackButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HackText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HackCost;
	
	UPROPERTY(meta = (BindWidget))
	UImage* HackImage;

private:

	// Hack this specific button is bound to
	TSubclassOf<UGameplayAbility> AssignedHack;
	
public:
	
	void Init(FString& _HackText, FString& _HackCost, UTexture2D& _HackImage, TSubclassOf<UGameplayAbility>& _AssignedHack);

private:

	UFUNCTION()
	void DoHack();
};
