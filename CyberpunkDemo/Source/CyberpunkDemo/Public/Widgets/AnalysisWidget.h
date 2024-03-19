// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Widgets/QuickhackWidget.h"
#include "Environment/HackableObjects/HackableComponent.h"
#include "Utility/DataStructures/FQuickhackDataStructure.h"
#include "AnalysisWidget.generated.h"

//class UQuickhackSystemComponent;
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

private:
	
	TSubclassOf<UQuickhackWidget> QuickhackWidgetClass;

	UPROPERTY()
	UDataTable* QuickhackDataTable;

public:

	void Init(UDataTable* _QuickhackDataTable, TSubclassOf<UQuickhackWidget> _QuickhackWidgetClass);

	void CreateHacks(const UHackableComponent* HackTarget);

	void RemoveHacks();
};
