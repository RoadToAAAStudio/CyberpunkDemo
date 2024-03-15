// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_ClearBlackboard.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ClearBlackboard::UBTDecorator_ClearBlackboard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Clear Blackboard");
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

void UBTDecorator_ClearBlackboard::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	Super::OnNodeDeactivation(SearchData, NodeResult);

	SearchData.OwnerComp.GetBlackboardComponent()->ClearValue(BlackboardKey.SelectedKeyName);
}
