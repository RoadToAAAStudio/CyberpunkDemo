// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "AnalysisWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UAnalysisWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget)) 
	UVerticalBox* QuickhacksVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InformationVerticalBox;

protected:

	virtual void NativePreConstruct() override;
};
