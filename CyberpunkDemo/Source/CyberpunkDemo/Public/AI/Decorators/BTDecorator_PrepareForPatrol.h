// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_PrepareForPatrol.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UBTDecorator_PrepareForPatrol : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};
