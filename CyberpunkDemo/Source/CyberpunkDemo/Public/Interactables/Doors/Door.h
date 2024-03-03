// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UnlockCondition.h"
#include "GameFramework/Actor.h"
#include "Interactables/IInspectable.h"
#include "Door.generated.h"

UCLASS()
class CYBERPUNKDEMO_API ADoor : public AActor, public IInspectable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UUnlockCondition>> Conditions;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ActiveState;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ADoor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Open();
	void Close();
	void Unlock();
	
	void Inspect() override;
	
	UFUNCTION(BlueprintCallable)
	void Interact();
};
