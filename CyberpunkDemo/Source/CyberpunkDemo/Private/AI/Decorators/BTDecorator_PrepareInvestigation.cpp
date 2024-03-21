// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_PrepareInvestigation.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PrepareInvestigation::UBTDecorator_PrepareInvestigation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Prepare for Investigation");
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

void UBTDecorator_PrepareInvestigation::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	UObject* KeyValue = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor")));
	AActor* Actor = Cast<AActor>(KeyValue);
	if (!Actor) return;

	ABasicEnemy* Enemy = Cast<ABasicEnemy>(Actor);
	if (!Enemy) return;

	//ABasicEnemyController* Controller = Cast<ABasicEnemyController>(Enemy->GetController());

	// TODO Check if there is something to investigate
}
