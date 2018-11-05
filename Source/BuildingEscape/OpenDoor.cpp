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

	if (PedastalActors.Num() < 1) {
		UE_LOG(LogTemp, Error, TEXT("Empty PedastalActors on %s"), *GetOwner()->GetName());
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
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
