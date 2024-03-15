// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/Interactables/Doors/UnlockCondition.h"
#include "UObject/Object.h"
#include "QuickHackCondition.generated.h"

UCLASS()
class CYBERPUNKDEMO_API UQuickHackCondition : public UUnlockCondition
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UQuickHackCondition();

	bool Check() override;
};
