// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Doors/UnlockCondition.h"
#include "UObject/Object.h"
#include "HighTechCondition.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UHighTechCondition : public UUnlockCondition
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UHighTechCondition();

	bool Check() override;
};
