// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HackText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HackCost;
	
	UPROPERTY(meta = (BindWidget))
	UImage* HackImage;

	void Init(FString& _HackText, FString& _HackCost, UTexture2D& _HackImage);
};
