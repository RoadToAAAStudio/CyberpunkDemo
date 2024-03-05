// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_MoveAlongSpline.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_MoveAlongSpline)

UBTTask_MoveAlongSpline::UBTTask_MoveAlongSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Move Along Spline";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveAlongSpline, BlackboardKey), AActor::StaticClass());
}

void UBTTask_MoveAlongSpline::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UBTTask_MoveAlongSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	UObject* KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* Actor = Cast<AActor>(KeyValue);
	if (!Actor) return EBTNodeResult::Failed;
	
	UActorComponent* ActorComponent = Actor->GetComponentByClass(USplineComponent::StaticClass());
	if (!ActorComponent) return EBTNodeResult::Failed;
	
	Spline = Cast<USplineComponent>(ActorComponent);
	ACharacter* Owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	float speed = MovementComponent->MaxWalkSpeed;

	// Spline->GetLocationAtDistanceAlongSpline()
	// Spline->GetSplineLength();
	// Owner->SetActorLocationAndRotation();
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}
