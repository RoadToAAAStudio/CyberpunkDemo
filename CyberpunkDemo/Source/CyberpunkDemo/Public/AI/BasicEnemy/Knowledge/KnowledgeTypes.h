// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "KnowledgeTypes.generated.h"

class ASplineContainer;
class AMainCharacter;
class USplineComponent;

USTRUCT(BlueprintType)
struct FSettableVector
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FVector Data = FVector(INFINITY, INFINITY, INFINITY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet = false;
	
	CYBERPUNKDEMO_API void Set(const FVector& Vector);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const FVector& Get() const;
	CYBERPUNKDEMO_API FVector& Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(const FVector& Vector);
};

USTRUCT(BlueprintType)
struct FSettableRotator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FRotator Data = FRotator(INFINITY, INFINITY, INFINITY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet = false;
	
	CYBERPUNKDEMO_API void Set(const FRotator& Rotator);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const FRotator& Get() const;
	CYBERPUNKDEMO_API FRotator& Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(const FRotator& Rotator);
};

USTRUCT(BlueprintType)
struct FSettableFloat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	float Data = INFINITY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet	= false;

	CYBERPUNKDEMO_API void Set(float Float);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API float Get() const;
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(float Float);
};

USTRUCT(BlueprintType)
struct FSettableInt
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int Data = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet = false;
	
	CYBERPUNKDEMO_API void Set(int Int);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API int Get() const;
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(int Int);
};

USTRUCT(BlueprintType)
struct FSettablePawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TObjectPtr<APawn> Data = nullptr;

	CYBERPUNKDEMO_API void Set(APawn* Object);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const APawn* Get() const;
	CYBERPUNKDEMO_API APawn* Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(APawn* Object);
};

USTRUCT(BlueprintType)
struct FSettableSpline
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TObjectPtr<ASplineContainer> Data = nullptr;

	CYBERPUNKDEMO_API void Set(ASplineContainer* Object);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const ASplineContainer* Get() const;
	CYBERPUNKDEMO_API ASplineContainer* Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(ASplineContainer* Object);
};

USTRUCT(BlueprintType)
struct FSettableMainCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TObjectPtr<AMainCharacter> Data = nullptr;

	CYBERPUNKDEMO_API void Set(AMainCharacter* Object);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const AMainCharacter* Get() const;
	CYBERPUNKDEMO_API AMainCharacter* Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(AMainCharacter* Object);
};

USTRUCT(BlueprintType)
struct FSettableAIStimulus
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FAIStimulus Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet = false;

	CYBERPUNKDEMO_API void Set(const FAIStimulus& Stimulus);
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const FAIStimulus& Get() const;
	CYBERPUNKDEMO_API FAIStimulus& Get();
	CYBERPUNKDEMO_API bool IsSet() const;
	CYBERPUNKDEMO_API void operator=(const FAIStimulus& Stimulus);
};

USTRUCT(BlueprintType)
struct FSettableTimerHandle
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FTimerHandle Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bIsSet = false;

	CYBERPUNKDEMO_API void Set();
	CYBERPUNKDEMO_API void UnSet();
	CYBERPUNKDEMO_API const FTimerHandle& Get() const;
	CYBERPUNKDEMO_API FTimerHandle& Get();
	CYBERPUNKDEMO_API bool IsSet() const;
};
