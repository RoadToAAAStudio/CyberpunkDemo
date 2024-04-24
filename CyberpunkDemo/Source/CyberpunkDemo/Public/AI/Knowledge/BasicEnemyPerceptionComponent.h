// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "BasicEnemyPerceptionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UBasicEnemyPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	void SetUpFromData(const UDataTable* ConfigData);
	
	UFUNCTION(BlueprintCallable)    TArray<UAISenseConfig*>&	GetSenses();
	UFUNCTION(BlueprintCallable)	UAISenseConfig*				GetSense(TSubclassOf<UAISenseConfig> Type);
};
