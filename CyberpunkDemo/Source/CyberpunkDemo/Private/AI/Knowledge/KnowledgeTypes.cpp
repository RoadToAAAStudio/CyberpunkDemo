// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/KnowledgeTypes.h"

// Vector -------------------------------------------------------------------
void FSettableVector::Set(const FVector& Vector)
{
	Data = Vector;
	bIsSet = true;
}

void FSettableVector::UnSet()
{
	Data = FVector(INFINITY, INFINITY, INFINITY);
	bIsSet = false;
}

const FVector& FSettableVector::Get() const
{
	return	Data;
}

bool FSettableVector::IsSet() const
{
	return	bIsSet;
}

void FSettableVector::operator=(const FVector& Vector)
{
	Data = Vector;
	bIsSet = true;
}

// Rotator ------------------------------------------------------------------
void FSettableRotator::Set(const FRotator& Rotator)
{
	Data = Rotator;
	bIsSet = true;
}

void FSettableRotator::UnSet()
{
	bIsSet = false;
}

const FRotator& FSettableRotator::Get() const
{
	return Data;
}

bool FSettableRotator::IsSet() const
{
	return bIsSet;
}

void FSettableRotator::operator=(const FRotator& Rotator)
{
	Data = Rotator;
	bIsSet = true;
}


// Float ---------------------------------------------------------------------
void FSettableFloat::Set(float Float)
{
	Data = Float;
	bIsSet = true;
}

void FSettableFloat::UnSet()
{
	Data = INFINITY;
	bIsSet = false;
}

float FSettableFloat::Get() const
{
	return Data;
}
bool FSettableFloat::IsSet() const
{
	return bIsSet;
}

void FSettableFloat::operator=(float Float)
{
	Data = Float;
	bIsSet = true;
}

// Int ---------------------------------------------------------------------
void FSettableInt::Set(int Int)
{
	Data = Int;
	bIsSet = true;
}

void FSettableInt::UnSet()
{
	Data = INT32_MAX;
	bIsSet = false;
}

int FSettableInt::Get() const
{
	return Data;
}

bool FSettableInt::IsSet() const
{
	return bIsSet;
}

void FSettableInt::operator=(int Int)
{
	Data = Int;
	bIsSet = true;
}

// Settable Pawn ----------------------------------------------------------------
void FSettablePawn::Set(APawn* Object)
{
	Data = Object;
}

void FSettablePawn::UnSet()
{
	Data = nullptr;
}

APawn* FSettablePawn::Get() const
{
	return Data;
}

bool FSettablePawn::IsSet() const
{
	return Data != nullptr;
}

void FSettablePawn::operator=(APawn* Object)
{
	Data = Object;
}

// Settable Spline --------------------------------------------------------------
void FSettableSpline::Set(ASplineContainer* Object)
{
	Data = Object;
}

void FSettableSpline::UnSet()
{
	Data = nullptr;
}

ASplineContainer* FSettableSpline::Get() const
{
	return Data;
}

bool FSettableSpline::IsSet() const
{
	return Data != nullptr;
}

void FSettableSpline::operator=(ASplineContainer* Object)
{
	Data = Object;
}

// Settable Main Character -------------------------------------------------------
void FSettableMainCharacter::Set(AMainCharacter* Object)
{
	Data = Object;
}

void FSettableMainCharacter::UnSet()
{
	Data = nullptr;
}

AMainCharacter* FSettableMainCharacter::Get() const
{
	return Data;
}

bool FSettableMainCharacter::IsSet() const
{
	return Data != nullptr;
}

void FSettableMainCharacter::operator=(AMainCharacter* Object)
{
	Data = Object;
}

// AIStimulus -------------------------------------------------------------------
void FSettableAIStimulus::Set(const FAIStimulus& Stimulus)
{
	Data = Stimulus;
	bIsSet = true;
}

void FSettableAIStimulus::UnSet()
{
	bIsSet = false;
}

const FAIStimulus& FSettableAIStimulus::Get() const
{
	return	Data;
}

bool FSettableAIStimulus::IsSet() const
{
	return	bIsSet;
}

void FSettableAIStimulus::operator=(const FAIStimulus& Stimulus)
{
	Data = Stimulus;
	bIsSet = true;
}

// TimerHandle -------------------------------------------------------------------
void FSettableTimerHandle::Set()
{
	bIsSet = true;
}

void FSettableTimerHandle::UnSet()
{
	bIsSet = false;
}

FTimerHandle& FSettableTimerHandle::Get()
{
	return Data;
}

bool FSettableTimerHandle::IsSet() const
{
	return Data.IsValid();
}
