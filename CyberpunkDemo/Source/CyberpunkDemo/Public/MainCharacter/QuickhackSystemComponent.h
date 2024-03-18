// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Environment//Interfaces/Hackerable.h"
#include "Environment/HackableObjects/HackableComponent.h"
#include "Widgets/AnalysisWidget.h"
#include "QuickhackSystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UQuickhackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quickhack Widget")
	TSubclassOf<UAnalysisWidget> AnalysisWidgetClass;
	
private:
	// Parameters to ignore when doing the raycast (set by the component owner)
	FCollisionQueryParams Params;

	UPROPERTY(EditDefaultsOnly, Category = "Quickhack")
	float QuickHackDistance = 2000.0f;

	UPROPERTY()
	UAnalysisWidget* AnalysisWidget;

	UPROPERTY()
	UHackableComponent* HackableComponentTarget;

public:
	// Sets default values for this component's properties
	UQuickhackSystemComponent();

	void Inspect();

	void CreateHacks(const UHackableComponent* HackTarget);

	void HandleAnalysisWidget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ResetHackTarget();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetIgnoredParams(const FCollisionQueryParams ParamsToIgnore);
};
