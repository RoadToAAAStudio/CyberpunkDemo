// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/InteractionSystemComponent.h"
#include "Environment/Interactables/IInspectable.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInteractionSystemComponent::UInteractionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InteractionMappingContext, 0);
		}
	}

	
}


// Called every frame
void UInteractionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Inspect();
}
//return tobjptr aactor
void UInteractionSystemComponent::Inspect()
{
	InspectedActor = nullptr;
	FVector Start = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector Forward = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
	Start = FVector(Start.X + (Forward.X * 50),Start.Y + (Forward.Y * 50),Start.Z + (Forward.Z * 50));
	FVector End = Start + (Forward * InteractionDistance);

	if (GetWorld())
	{
		FHitResult Hit;
		//right channel ?
		bool actorHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility,FCollisionQueryParams(), FCollisionResponseParams());
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow,false,0,0,0.1f);
		if (actorHit)
		{
			//Set GameplayTag
			InspectedActor = Hit.GetActor();
			if (Cast<IInspectable>(InspectedActor))
			{
				DrawDebugSphere(GetWorld(),Hit.Location, 10, 16, FColor::Green ,false, 0,0,1);
				GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, TEXT("It's inspectable!"));
				Cast<IInspectable>(InspectedActor)->Inspect();
			}
			else
			{
				DrawDebugSphere(GetWorld(),Hit.Location, 10, 16, FColor::Yellow ,false, 0,0,1);
			}
		}
	}
}

void UInteractionSystemComponent::Interact()
{
	
}


