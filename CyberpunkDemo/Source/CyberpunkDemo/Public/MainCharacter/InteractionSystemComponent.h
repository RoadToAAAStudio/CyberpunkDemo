// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "InteractionSystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UInteractionSystemComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> InspectedActor;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer Tags;
	UPROPERTY(EditAnywhere)
	float InteractionDistance;
	
	
	// Sets default values for this component's properties
	UInteractionSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void Inspect();
	void Interact();
};
