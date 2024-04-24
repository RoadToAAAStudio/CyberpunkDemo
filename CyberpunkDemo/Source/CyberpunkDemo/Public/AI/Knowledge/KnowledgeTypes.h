// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "KnowledgeTypes.generated.h"

class AMainCharacter;
class USplineComponent;

USTRUCT(BlueprintType)
struct FSettableVector
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly) FVector Data = FVector(INFINITY, INFINITY, INFINITY);
	UPROPERTY(BlueprintReadOnly) bool bIsSet = false;
	
public:
	void Set(const FVector& Vector);
	void UnSet();
	const FVector& Get() const;
	bool IsSet() const;
	void operator=(const FVector& Vector);
};

USTRUCT(BlueprintType)
struct FSettableFloat
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly) float Data = INFINITY;
	UPROPERTY(BlueprintReadOnly) bool bIsSet	= false;
public:
	void Set(float Float);
	void UnSet();
	float Get() const;
	bool IsSet() const;

	void operator=(float Float);
};

USTRUCT(BlueprintType)
struct FSettableInt
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly) int Data = INT32_MAX;
	UPROPERTY(BlueprintReadOnly) bool bIsSet = false;
	
public:
	void Set(int Int);
	void UnSet();
	int Get() const;
	bool IsSet() const;
	void operator=(int Int);
};

USTRUCT(BlueprintType)
struct FSettablePawn
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) TObjectPtr<APawn> Data = nullptr;
public:
	void Set(APawn* Object);
	void UnSet();
	APawn* Get() const;
	bool IsSet() const;
	void operator=(APawn* Object);
};

USTRUCT(BlueprintType)
struct FSettableSpline
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) TObjectPtr<USplineComponent> Data = nullptr;
public:
	void Set(USplineComponent* Object);
	void UnSet();
	USplineComponent* Get() const;
	bool IsSet() const;
	void operator=(USplineComponent* Object);
};

USTRUCT(BlueprintType)
struct FSettableMainCharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) TObjectPtr<AMainCharacter> Data = nullptr;
public:
	void Set(AMainCharacter* Object);
	void UnSet();
	AMainCharacter* Get() const;
	bool IsSet() const;
	void operator=(AMainCharacter* Object);
};

USTRUCT(BlueprintType)
struct FSettableAIStimulus
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly) FAIStimulus Data;
	UPROPERTY(BlueprintReadOnly) bool bIsSet = false;
public:
	void Set(const FAIStimulus& Stimulus);
	void UnSet();
	const FAIStimulus& Get() const;
	bool IsSet() const;
	void operator=(const FAIStimulus& Stimulus);
};

USTRUCT(BlueprintType)
struct FSettableTimerHandle
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly) FTimerHandle Data;
	UPROPERTY(BlueprintReadOnly) bool bIsSet = false;
public:
	void Set(const FTimerHandle& Timer);
	void UnSet();
	FTimerHandle& Get();
	bool IsSet() const;
	void operator=(const FTimerHandle& Timer);
};
