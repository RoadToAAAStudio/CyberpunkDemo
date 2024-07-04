// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/IInspectable.h"


// Add default functionality here for any IInspectable functions that are not pure virtual.
void IInspectable::Inspect()
{
	
}

TArray<EInspectInput> IInspectable::GetInspectInputs()
{
	return TArray<EInspectInput>();
}

TArray<bool> IInspectable::GetChecksResults()
{
	return TArray<bool>();
}


