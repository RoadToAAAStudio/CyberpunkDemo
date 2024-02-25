// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UnlockCondition.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class CYBERPUNKDEMO_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<AUnlockCondition*> Conditions;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ActiveState;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ADoor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Open();
	UFUNCTION()
	void Close();
	UFUNCTION()
	void Unlock();
	UFUNCTION()
	void Inspect();
	UFUNCTION()
	void Interact();
};
