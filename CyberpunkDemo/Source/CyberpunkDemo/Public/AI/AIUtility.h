// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StateTree.h"
#include "StateTreeExecutionTypes.h"
#include "AIUtility.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UAIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "AIUtility | StateTree")
	static FName GetCurrentState(const UStateTree* StateTree, const FStateTreeTransitionResult& Transition);

	UFUNCTION(BlueprintCallable, Category = "AIUtility | StateTree")
	static FName GetSourceState(const UStateTree* StateTree, const FStateTreeTransitionResult& Transition);
};
