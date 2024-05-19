// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "Components/StateTreeComponent.h"
#include "SettableStateTreeComponent.generated.h"

/**
 * A version of UStateTreeComponent which supports StateTree asset change at runtime
 */
UCLASS()
class CYBERPUNKDEMO_API USettableStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void SetTree(UStateTree* TargetTree);
};
