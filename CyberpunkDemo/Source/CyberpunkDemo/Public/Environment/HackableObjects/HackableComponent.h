// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "HackableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UHackableComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Hacks")
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks;

	UPROPERTY(EditAnywhere, Category = "Inspection Settings")
	float InspectionTime;
	
	bool bHasBeenInspected = false;

	bool bIsUnderInspection = false;

	FTimerHandle InspectionTimerHandle;

public:
	// Sets default values for this component's properties
	UHackableComponent();

	TSet<TSubclassOf<UGameplayAbility>> GetPossibleHacks() const;

	void StartInspectionTimer();

	void StopInspectionTimer();

	bool GetHasBeenInspected();
	
	bool GetIsUnderInspection();

	//void SetIsUnderInspection(bool UnderInspection);

	void Highlight() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetHasBeenInspected();
};
