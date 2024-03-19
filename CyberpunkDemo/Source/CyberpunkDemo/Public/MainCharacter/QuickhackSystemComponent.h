// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Environment/HackableObjects/HackableComponent.h"
#include "Widgets/AnalysisWidget.h"
#include "Widgets/QuickhackWidget.h"
#include "QuickhackSystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCompletedTargetAnalysisSignature, UHackableComponent*);
DECLARE_MULTICAST_DELEGATE(FOnFinishedTargetAnalysisSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UQuickhackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quickhack Widget")
	TSubclassOf<UAnalysisWidget> AnalysisWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quickhack Widget")
	TSubclassOf<UQuickhackWidget> QuickhackWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Quickhack Data")
	UDataTable* QuickhackDataTable;

	FOnCompletedTargetAnalysisSignature OnCompletedTargetAnalysis;
	FOnFinishedTargetAnalysisSignature OnFinishedTargetAnalysis;
	
private:
	// Parameters to ignore when doing the raycast (set by the component owner)
	FCollisionQueryParams Params;

	UPROPERTY(EditDefaultsOnly, Category = "Quickhack")
	float QuickHackDistance = 2000.0f;

	UPROPERTY()
	UAnalysisWidget* AnalysisWidget;

	UPROPERTY()
	UHackableComponent* HackableComponentTarget;

	UPROPERTY()
	UHackableComponent* PreviousHackableComponentTarget;

	bool bIsQuickhackCreated = false;

public:
	// Sets default values for this component's properties
	UQuickhackSystemComponent();

	void Inspect();

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
