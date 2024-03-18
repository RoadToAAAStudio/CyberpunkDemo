// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/QuickhackWidget.h"

void UQuickhackWidget::Init(FString& _HackText, FString& _HackCost, UTexture2D& _HackImage)
{
	HackText->SetText(FText::FromString(_HackText));
	HackCost->SetText(FText::FromString(_HackCost));
	HackImage->SetBrushFromTexture(&_HackImage);
}
