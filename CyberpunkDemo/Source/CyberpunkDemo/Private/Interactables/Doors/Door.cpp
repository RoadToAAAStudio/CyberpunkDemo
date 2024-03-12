// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Doors/Door.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::Open()
{
	//does it work?
	ActiveState = FGameplayTag::RequestGameplayTag("Environment.Interactables.Door.State.Opened");
}

void ADoor::Close()
{
	ActiveState = FGameplayTag::RequestGameplayTag("Environment.Interactables.Door.State.Closed");
}

void ADoor::Unlock()
{
	Conditions.Empty();
	ActiveState = FGameplayTag::RequestGameplayTag("Environment.Interactables.Door.State.Opened");
}

//must be called from inspection system, check if conditions are met and call relative UI
void ADoor::Inspect()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Inspecting door!"));
	if (Conditions.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Door is unlocked!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("checking door conditions!"));
		for (int i = 0; i < Conditions.Num(); i++)
		{
			if (!Conditions[i])
			{
				continue;
			}
			Conditions[i]->Check();
		}
	}
}

void ADoor::Interact()
{
	
	//must be called by an input and check if the condition is met to complete and unlock the door
}

