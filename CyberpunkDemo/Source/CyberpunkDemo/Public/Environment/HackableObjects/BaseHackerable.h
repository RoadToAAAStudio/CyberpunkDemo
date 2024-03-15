// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/Interfaces/Hackerable.h"
#include "GameFramework/Actor.h"
#include "BaseHackerable.generated.h"

UCLASS()
class CYBERPUNKDEMO_API ABaseHackerable : public AActor,  public IHackerable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Hacks")
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks;

public:
	// Sets default values for this actor's properties
	ABaseHackerable();

	virtual TSet<TSubclassOf<UGameplayAbility>> GetPossibleHacks() const override;

	virtual void Highlight() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
