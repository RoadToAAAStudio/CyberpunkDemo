// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/BTDecorator_PrepareForPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"

class UBlackboardKeyType_Object;

UBTDecorator_PrepareForPatrol::UBTDecorator_PrepareForPatrol(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Prepare for Patrol");
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

void UBTDecorator_PrepareForPatrol::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

	// Get Spline
	UObject* KeyValue = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("PatrolSpline")));
	AActor* Actor = Cast<AActor>(KeyValue);
	if (!Actor) return;
		
	UActorComponent* ActorComponent = Actor->GetComponentByClass(USplineComponent::StaticClass());
	if (!ActorComponent) return;
		
	USplineComponent* Spline = Cast<USplineComponent>(ActorComponent);
	if (!Spline)  return;

	// Get AI agent
	KeyValue = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor")));
	Actor = Cast<AActor>(KeyValue);
	if (!Actor) return;

	FVector FirstPointLocation = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector ActorLocation = Actor->GetActorLocation();

	// Set Target Location
	if (FVector::Distance(ActorLocation, FirstPointLocation) > 10.0f)
	{
		SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TargetLocation")), FirstPointLocation);
	}
}
