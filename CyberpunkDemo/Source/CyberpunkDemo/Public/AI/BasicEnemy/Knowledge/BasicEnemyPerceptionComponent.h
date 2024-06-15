// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BasicEnemyPerceptionComponent.generated.h"

class UBasicEnemyPerceptionData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UBasicEnemyPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	void SetUpFromData(const UBasicEnemyPerceptionData* PerceptionConfigData);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)		TArray<UAISenseConfig*>&	GetSenses();
	UFUNCTION(BlueprintCallable, BlueprintPure)		UAISenseConfig*				GetSense(TSubclassOf<UAISenseConfig> Type);
};
