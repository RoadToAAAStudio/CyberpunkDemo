// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/QuickhackWidget.h"

void UQuickhackWidget::Init(FString& _HackText, FString& _HackCost, UTexture2D& _HackImage, TSubclassOf<UGameplayAbility>& _AssignedHack)
{
	HackText->SetText(FText::FromString(_HackText));
	HackCost->SetText(FText::FromString(_HackCost));
	HackImage->SetBrushFromTexture(&_HackImage);
	AssignedHack = _AssignedHack;

	HackButton->OnReleased.AddDynamic(this, &UQuickhackWidget::DoHack);
}

void UQuickhackWidget::DoHack()
{
	FString Print = "Performing hack: ";
	Print.Append(AssignedHack->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, Print);
}
