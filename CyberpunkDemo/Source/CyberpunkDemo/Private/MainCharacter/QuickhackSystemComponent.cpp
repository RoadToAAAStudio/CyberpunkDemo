// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/QuickhackSystemComponent.h"

#include "Blueprint/UserWidget.h"


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
	IHackerable* HackerableTarget = nullptr;
	if (GetWorld()->LineTraceSingleByProfile(HitResult, LineStart, LineEnd, "BlockAll", Params))
	{
		if (Cast<IHackerable>(HitResult.GetActor()))
		{
			CreateHacks(Cast<IHackerable>(HitResult.GetActor()));
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Green, false, 2);
		}
		else
		{
			DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 2);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("NO VALID HIT"));
		}
	}
}

void UQuickhackSystemComponent::CreateHacks(const IHackerable* HackTarget)
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks = HackTarget->GetPossibleHacks();
	if (PossibleHacks.Num() <= 0) return;
	for (auto Hack : PossibleHacks)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString(Hack->GetName()));
	}
}

void UQuickhackSystemComponent::HandleAnalysisWidget()
{
	if (!AnalysisWidget)
	{
		AnalysisWidget = Cast<UAnalysisWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), UAnalysisWidget::StaticClass()));
		AnalysisWidget->AddToViewport();
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString("CREATING WIDGET"));
	}
	else
	{
		if (AnalysisWidget->IsInViewport())
		{
			AnalysisWidget->RemoveFromParent();
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("REMOVING WIDGET"));
		}
		else
		{
			AnalysisWidget->AddToViewport();
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString("ADDING WIDGET"));
		}
	}

}


// Called when the game starts
void UQuickhackSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuickhackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuickhackSystemComponent::SetIgnoredParams(const FCollisionQueryParams ParamsToIgnore)
{
	Params = ParamsToIgnore;
}

