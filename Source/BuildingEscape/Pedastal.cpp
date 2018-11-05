// Fill out your copyright notice in the Description page of Project Settings.

#include "Pedastal.h"
#include "Statue.h"

// Sets default values for this component's properties
UPedastal::UPedastal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPedastal::BeginPlay()
{
	Super::BeginPlay();

	if (CorrectStatueNumber < 0) {
		UE_LOG(LogTemp, Error, TEXT("Missing CorrectStatueNumber on %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UPedastal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPedastal::IsAnyStatueMounted(TArray<int> StatueNumbers)
{
	return StatueNumbers.Contains(CorrectStatueNumber);
}

bool UPedastal::IsCorrectStatueMounted()
{
	if (CorrectStatueNumber < 0) { return false; }

	bool IsCorrect = false;

	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OUT OverlappingActors);
	for (const auto& Actor : OverlappingActors) {
		UStatue* Statue = Actor->FindComponentByClass<UStatue>();
		if (Statue && Statue->GetStatueNumber() == CorrectStatueNumber) {
			IsCorrect = true;
			break;
		}
	}

	return IsCorrect;
}

int UPedastal::GetCorrectStatueNumber()
{
	return CorrectStatueNumber;
}


