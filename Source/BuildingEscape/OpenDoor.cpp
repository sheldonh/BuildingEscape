// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorState = Closed;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Missing PressurePlate on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPressurePlate() >= TotalMassRequiredToTrigger) {
		if (DoorState == Closing)
		{
			GetWorld()->GetTimerManager().ClearTimer(CloseDoorTimerHandle);
		}
		if (DoorState != Open)
		{
			DoorState = Open;
			OpenDoor();
		}
	}
	else
	{
		if (DoorState == Open)
		{
			DoorState = Closing;
			GetWorld()->GetTimerManager().SetTimer(CloseDoorTimerHandle, this, &UOpenDoor::CloseDoor, CloseDelay, false);
		}
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPressurePlate()
{
	if (!PressurePlate) { return 0.0; }

	float TotalMass = 0.0;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (auto& Actor : OverlappingActors) {
		auto PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();
		TotalMass += PrimitiveComponent->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	SetDoorAngle(OpenAngle);
}

void UOpenDoor::CloseDoor()
{
	DoorState = Closed;
	SetDoorAngle(CloseAngle);
}

void UOpenDoor::SetDoorAngle(float angle)
{
	AActor* Owner = GetOwner();
	FRotator OldRotation = Owner->GetTransform().GetRotation().Rotator();
	FRotator NewRotation = FRotator(OldRotation.Pitch, angle, OldRotation.Roll);
	Owner->SetActorRotation(NewRotation);
}
