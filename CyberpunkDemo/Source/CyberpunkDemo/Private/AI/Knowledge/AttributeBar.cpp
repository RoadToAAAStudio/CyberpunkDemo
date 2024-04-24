// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/AttributeBar.h"

void UAttributeBar::Add(float Amount)
{
	if (Amount < 0 || Value == 1) return;
	Value += Amount;
	Value = FMath::Clamp(Value, 0.0f, 1.0f);

	if (Value == 1)
	{
		bIsFull = true;
		OnBarFilledDelegate.Broadcast();
	}
}

void UAttributeBar::Remove(float Amount)
{
	if (Amount < 0 || Value == 0) return;
	Value -= Amount;
	Value = FMath::Clamp(Value, 0.0f, 1.0f);

	if (Value < 1)
	{
		bIsFull = false;
	}
	if (Value == 0)
	{
		OnBarEmptiedDelegate.Broadcast();
	}
}

bool UAttributeBar::IsFull() const { return bIsFull; }

float UAttributeBar::GetValue() const { return Value; }

void UAttributeBar::Fill()
{
	Value = 1;
	bIsFull = true;
}

void UAttributeBar::Reset()
{
	Value = 0;
	bIsFull = false;
}

