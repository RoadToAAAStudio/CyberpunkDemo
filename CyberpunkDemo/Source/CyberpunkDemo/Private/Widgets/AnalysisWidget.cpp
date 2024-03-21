// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/AnalysisWidget.h"

#include "MainCharacter/MainCharacter.h"
#include "MainCharacter/QuickhackSystemComponent.h"


void UAnalysisWidget::Init(UQuickhackSystemComponent* _Owner, UDataTable* _QuickhackDataTable, TSubclassOf<UQuickhackWidget> _QuickhackWidgetClass)
{
	Owner = _Owner;
	Owner->OnCompletedTargetAnalysis.AddUObject(this, &UAnalysisWidget::CreateHacks);
	Owner->OnFinishedTargetAnalysis.AddUObject(this, &UAnalysisWidget::RemoveHacks);
	QuickhackDataTable = _QuickhackDataTable;
	QuickhackWidgetClass = _QuickhackWidgetClass;
	PlayerHacks = Cast<AMainCharacter>(GetOwningLocalPlayer())->GetPlayerHacks();
}

void UAnalysisWidget::CreateHacks(UHackableComponent* HackTarget)
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks = HackTarget->GetPossibleHacks();
	TSet<TSubclassOf<UGameplayAbility>> EnabledHacks = PossibleHacks.Intersect(PlayerHacks);
	if (PossibleHacks.Num() <= 0) return;
	for (auto Hack : EnabledHacks)
	{
		UQuickhackWidget* QuickhackWidget = Cast<UQuickhackWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), QuickhackWidgetClass));
		FQuickhackDataStructure* QuickhackData = QuickhackDataTable->FindRow<FQuickhackDataStructure>(Hack->GetFName(), "");
		if (!QuickhackData)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Ability not found!"));
			return;
		}
		QuickhacksVerticalBox->AddChild(QuickhackWidget);
		QuickhackWidget->Init(QuickhackData->HackName, QuickhackData->HackCost, *QuickhackData->HackImage, Hack);
		QuickhackWidget->AddToViewport();
	}
}

void UAnalysisWidget::RemoveHacks()
{
	QuickhacksVerticalBox->ClearChildren();
}
