// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Environment//Interfaces/Hackerable.h"
#include "QuickhackSystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UQuickhackSystemComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

	// Parameters to ignore when doing the raycast (set by the component owner)
	FCollisionQueryParams Params;

	//TObjectPtr<IHackerable> HackTarget;
	
public:
	// Sets default values for this component's properties
	UQuickhackSystemComponent();

	void Inspect();

	void CreateHacks(const IHackerable* HackTarget);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
