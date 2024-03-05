// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "SplineContainer.generated.h"

/*
 * Class that holds a spline
 */

UCLASS()
class CYBERPUNKDEMO_API ASplineContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineContainer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineComponent> Spline;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
