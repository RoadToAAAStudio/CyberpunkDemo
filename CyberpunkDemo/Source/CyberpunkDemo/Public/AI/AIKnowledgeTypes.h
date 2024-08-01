// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIKnowledgeTypes.generated.h"

class ASplineContainer;
class AMainCharacter;
class USplineComponent;

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableVector
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Data = FVector(INFINITY, INFINITY, INFINITY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set(const FVector& Vector);
	FORCEINLINE void UnSet();
	FORCEINLINE const FVector& Get() const;
	FORCEINLINE FVector& Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(const FVector& Vector);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableRotator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Data = FRotator(INFINITY, INFINITY, INFINITY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set(const FRotator& Rotator);
	FORCEINLINE void UnSet();
	FORCEINLINE const FRotator& Get() const;
	FORCEINLINE FRotator& Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(const FRotator& Rotator);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableFloat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Data = INFINITY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set(float Float);
	FORCEINLINE void UnSet();
	FORCEINLINE float Get() const;
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(float Float);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableInt
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Data = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set(int Int);
	FORCEINLINE void UnSet();
	FORCEINLINE int Get() const;
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(int Int);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettablePawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> Data = nullptr;

	FORCEINLINE void Set(APawn* Object);
	FORCEINLINE void UnSet();
	FORCEINLINE const APawn* Get() const;
	FORCEINLINE APawn* Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(APawn* Object);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableSpline
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASplineContainer> Data = nullptr;

	FORCEINLINE void Set(ASplineContainer* Object);
	FORCEINLINE void UnSet();
	FORCEINLINE const ASplineContainer* Get() const;
	FORCEINLINE ASplineContainer* Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(ASplineContainer* Object);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableMainCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AMainCharacter> Data = nullptr;

	FORCEINLINE void Set(AMainCharacter* Object);
	FORCEINLINE void UnSet();
	FORCEINLINE const AMainCharacter* Get() const;
	FORCEINLINE AMainCharacter* Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(AMainCharacter* Object);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableAIStimulus
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAIStimulus Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set(const FAIStimulus& Stimulus);
	FORCEINLINE void UnSet();
	FORCEINLINE const FAIStimulus& Get() const;
	FORCEINLINE FAIStimulus& Get();
	FORCEINLINE bool IsSet() const;
	FORCEINLINE void operator=(const FAIStimulus& Stimulus);
};

USTRUCT(BlueprintType)
struct CYBERPUNKDEMO_API FSettableTimerHandle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSet = false;

	FORCEINLINE void Set();
	FORCEINLINE void UnSet();
	FORCEINLINE const FTimerHandle& Get() const;
	FORCEINLINE FTimerHandle& Get();
	FORCEINLINE bool IsSet() const;
};
