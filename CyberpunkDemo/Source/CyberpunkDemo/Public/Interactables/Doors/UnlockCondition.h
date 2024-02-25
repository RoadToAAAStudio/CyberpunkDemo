// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnlockCondition.generated.h"

UCLASS(Abstract)
class CYBERPUNKDEMO_API AUnlockCondition : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnlockCondition();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual bool Interact();
};
