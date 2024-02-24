// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utility/StateInterface.h"
#include "Walking.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API UWalking : public UObject, public IStateInterface
{
	GENERATED_BODY()
};
