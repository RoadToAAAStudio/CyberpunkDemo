// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/Interactables/Doors/UnlockCondition.h"
#include "UObject/Object.h"
#include "AccessCardCondition.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UAccessCardCondition : public UUnlockCondition
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UAccessCardCondition();
	
	bool Check() override;
};
