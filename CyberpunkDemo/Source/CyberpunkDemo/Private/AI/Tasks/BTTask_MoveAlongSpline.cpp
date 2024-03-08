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
	FBTMoveAlongSplineMemory* MyMemory = (FBTMoveAlongSplineMemory*) NodeMemory;
	if (!MyMemory) return;
	ACharacter* Owner = MyMemory->Owner;
	if (!Owner) return;
	
	float Speed = MyMemory->MovementComponent->MaxWalkSpeed;
	float SplineLength = MyMemory->Spline->GetSplineLength();
	float PositionOnSpline = MyMemory->PositionOnSpline;
	float PositionOnSplineInSpaceUnits = PositionOnSpline * SplineLength;
	if (!bStartsFromEnd)
	{
		// if (SplineLength - PositionOnSplineInSpaceUnits <= StartToSlowDownDistance)
		// {
		// 	Speed = FMath::Lerp(Speed, Speed / 2, FMath::GetMappedRangeValueClamped(UE::Math::TVector2<float>(StartToSlowDownDistance, 0.0f), UE::Math::TVector2(0.0f, 1.0f), SplineLength - PositionOnSplineInSpaceUnits));
		// }
		PositionOnSplineInSpaceUnits += Speed * DeltaSeconds;
	}
	else
	{
		// if (PositionOnSplineInSpaceUnits <= StartToSlowDownDistance)
		// {
		// 	Speed = FMath::Lerp(Speed, Speed / 2, FMath::GetMappedRangeValueClamped(UE::Math::TVector2<float>(StartToSlowDownDistance, 0.0f), UE::Math::TVector2(0.0f, 1.0f), PositionOnSplineInSpaceUnits));
		// }
		PositionOnSplineInSpaceUnits += (-1) * Speed * DeltaSeconds;
	}
	MyMemory->PositionOnSpline = FMath::Clamp(PositionOnSplineInSpaceUnits / SplineLength, 0.0f, 1.0f);
	FVector SplineLocation = MyMemory->Spline->GetLocationAtDistanceAlongSpline(PositionOnSplineInSpaceUnits, ESplineCoordinateSpace::World);
	SplineLocation.Set(SplineLocation.X, SplineLocation.Y, Owner->GetActorLocation().Z);
	Owner->SetActorLocation(SplineLocation);
	FRotator SplineRotator = MyMemory->Spline->GetRotationAtDistanceAlongSpline(PositionOnSplineInSpaceUnits, ESplineCoordinateSpace::Local);
	SplineRotator.Yaw = bStartsFromEnd? SplineRotator.Yaw - 180.0f : SplineRotator.Yaw;
	Owner->SetActorRelativeRotation(SplineRotator.Quaternion());

	if (!bStartsFromEnd)
	{
		if (MyMemory->PositionOnSpline >= 1.0f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if (MyMemory->PositionOnSpline <= 0.0f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UBTTask_MoveAlongSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveAlongSplineMemory* MyMemory = CastInstanceNodeMemory<FBTMoveAlongSplineMemory>(NodeMemory);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	UObject* KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* Actor = Cast<AActor>(KeyValue);
	if (!Actor) return EBTNodeResult::Failed;
	
	UActorComponent* ActorComponent = Actor->GetComponentByClass(USplineComponent::StaticClass());
	if (!ActorComponent) return EBTNodeResult::Failed;
	
	USplineComponent* Spline = Cast<USplineComponent>(ActorComponent);
	if (!Spline)  return EBTNodeResult::Failed;
	
	ACharacter* Owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Owner) return EBTNodeResult::Failed;
	
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	if (!MovementComponent) return EBTNodeResult::Failed;

	MyMemory->Owner = Owner;
	MyMemory->MovementComponent = MovementComponent;
	MyMemory->Spline = Spline;
	MyMemory->PositionOnSpline = bStartsFromEnd ? 1.0f : 0.0f;

	return EBTNodeResult::InProgress;
}

uint16 UBTTask_MoveAlongSpline::GetInstanceMemorySize() const
{
	return sizeof(FBTMoveAlongSplineMemory);
}
