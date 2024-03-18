// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/QuickhackSystemComponent.h"

#include "Blueprint/UserWidget.h"
#include "Environment/HackableObjects/HackableComponent.h"


// Sets default values for this component's properties
UQuickhackSystemComponent::UQuickhackSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UQuickhackSystemComponent::Inspect()
{
	FVector LineStart = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector LineEnd = LineStart + GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector() * QuickHackDistance;
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByProfile(HitResult, LineStart, LineEnd, "BlockAll", Params))
	{
		if (HitResult.GetActor()->GetComponentByClass(UHackableComponent::StaticClass()))
		{
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green, false, 2);
			HackableComponentTarget = Cast<UHackableComponent>(HitResult.GetActor()->GetComponentByClass(UHackableComponent::StaticClass()));
			if (!HackableComponentTarget->GetHasBeenInspected() && !HackableComponentTarget->GetIsUnderInspection())
			{
				HackableComponentTarget->StartInspectionTimer();
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString("INSPECTING"));
			}
			else if (HackableComponentTarget->GetHasBeenInspected())
			{
				CreateHacks(Cast<UHackableComponent>(HitResult.GetActor()->GetComponentByClass(UHackableComponent::StaticClass())));
			}
		}
		else
		{
			ResetHackTarget();
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 2);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("NO VALID HIT"));
		}
	}
}

// TODO: Fix method is called on tick

void UQuickhackSystemComponent::CreateHacks(const UHackableComponent* HackTarget)
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks = HackTarget->GetPossibleHacks();
	if (PossibleHacks.Num() <= 0) return;
	for (auto Hack : PossibleHacks)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString(Hack->GetName()));
		UQuickhackWidget* QuickhackWidget = Cast<UQuickhackWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), QuickhackWidgetClass));
		AnalysisWidget->QuickhacksVerticalBox->AddChildToVerticalBox(QuickhackWidget);
		FQuickHackData* QuickhackData = QuickhackDataTable->FindRow<FQuickHackData>(Hack->GetFName(), "");
		QuickhackWidget->Init(QuickhackData->HackName, QuickhackData->HackCost, *QuickhackData->HackImage);
		QuickhackWidget->AddToViewport();
	}
}

void UQuickhackSystemComponent::HandleAnalysisWidget()
{
	if (!AnalysisWidget)
	{
		AnalysisWidget = Cast<UAnalysisWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), AnalysisWidgetClass));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString("CREATING WIDGET"));
	}

	if (AnalysisWidget->IsInViewport())
	{
		AnalysisWidget->RemoveFromParent();
		ResetHackTarget();
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetFOV(100);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("REMOVING WIDGET"));
	}
	else
	{
		AnalysisWidget->AddToViewport();
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetFOV(70);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString("ADDING WIDGET"));
	}
}


// Called when the game starts
void UQuickhackSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UQuickhackSystemComponent::ResetHackTarget()
{
	if (HackableComponentTarget)
	{
		HackableComponentTarget->StopInspectionTimer();
		HackableComponentTarget = nullptr;
	}
}


// Called every frame
void UQuickhackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AnalysisWidget)
	{
		if (AnalysisWidget->IsInViewport())
		{
			Inspect();
		}
	}
}

void UQuickhackSystemComponent::SetIgnoredParams(const FCollisionQueryParams ParamsToIgnore)
{
	Params = ParamsToIgnore;
}

