// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "DoorLight.h"
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

	if (PedastalActors.Num() < 1) {
		UE_LOG(LogTemp, Warning, TEXT("Empty PedastalActors on %s"), *GetOwner()->GetName());
	} else {
		for (const auto& PedastalActor : PedastalActors) {
			UPedastal* Pedastal = PedastalActor->FindComponentByClass<UPedastal>();
			if (!Pedastal) {
				UE_LOG(LogTemp, Error, TEXT("Missing Pedastal on PedastalActor %s in %s"), *PedastalActor->GetName(), *GetOwner()->GetName());
			} else {
				Pedastals.Add(Pedastal);
			}
		}
	}

	if (DoorLightActors.Num() < 1) {
		UE_LOG(LogTemp, Warning, TEXT("Empty DoorLightActors on %s"), *GetOwner()->GetName());
	}
	else {
		for (const auto& DoorLightActor : DoorLightActors) {
			UDoorLight* DoorLight = DoorLightActor->FindComponentByClass<UDoorLight>();
			if (!DoorLight) {
				UE_LOG(LogTemp, Error, TEXT("Missing DoorLight on DoorLightActor %s in %s"), *DoorLightActor->GetName(), *GetOwner()->GetName());
			}
			else {
				DoorLights.Add(DoorLight);
			}
		}
	}

	if (DoorLightActors.Num() != PedastalActors.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Mismatched number of DoorlightActors and PedastalActors on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetLightColors();
	if (AreAllPedastalsCorrectlyMounted()) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}
}

bool UOpenDoor::AreAllPedastalsCorrectlyMounted()
{
	bool AllCorrect = true;

	for (const auto& Pedastal : Pedastals) {
		if (!Pedastal->IsCorrectStatueMounted()) {
			AllCorrect = false;
		}
	}

	return AllCorrect;
}

void UOpenDoor::SetLightColors()
{
	int Correct = 0;
	int Useful = 0;

	// TODO Move to member
	TArray<int> StatueNumbers;
	for (const auto& Pedastal : Pedastals) {
		StatueNumbers.Add(Pedastal->GetCorrectStatueNumber());
	}

	for (const auto& Pedastal : Pedastals) {
		if (Pedastal->IsCorrectStatueMounted()) {
			Correct++;
		} else if (Pedastal->IsAnyStatueMounted(StatueNumbers)) {
			Useful++;
		}
	}

	for (const auto& DoorLight : DoorLights) {
		if (Correct > 0) {
			Correct--;
			DoorLight->SetColor(UDoorLight::Green);
		}
		else if (Useful > 0) {
			Useful--;
			DoorLight->SetColor(UDoorLight::Amber);
		} else {
			DoorLight->SetColor(UDoorLight::Off);
		}
	}
}