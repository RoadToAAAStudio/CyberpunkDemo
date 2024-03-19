// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AnalysisWidget.h"

void UAnalysisWidget::Init(UDataTable* _QuickhackDataTable,
                           TSubclassOf<UQuickhackWidget> _QuickhackWidgetClass)
{
	QuickhackDataTable = _QuickhackDataTable;
	QuickhackWidgetClass = _QuickhackWidgetClass;
}

void UAnalysisWidget::CreateHacks(const UHackableComponent* HackTarget)
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleHacks = HackTarget->GetPossibleHacks();
	if (PossibleHacks.Num() <= 0) return;
	for (auto Hack : PossibleHacks)
	{
		UQuickhackWidget* QuickhackWidget = Cast<UQuickhackWidget>(CreateWidget(GetWorld()->GetFirstPlayerController(), QuickhackWidgetClass));
		FQuickhackDataStructure* QuickhackData = QuickhackDataTable->FindRow<FQuickhackDataStructure>(Hack->GetFName(), "");
		QuickhacksVerticalBox->AddChildToVerticalBox(QuickhackWidget);
		QuickhackWidget->Init(QuickhackData->HackName, QuickhackData->HackCost, *QuickhackData->HackImage);
		QuickhackWidget->AddToViewport();
	}
}

void UAnalysisWidget::RemoveHacks()
{
	QuickhacksVerticalBox->ClearChildren();
}
