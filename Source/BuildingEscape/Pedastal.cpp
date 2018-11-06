// Fill out your copyright notice in the Description page of Project Settings.

#include "Pedastal.h"

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

bool UPedastal::IsAnyStatueMounted()
{

	return GetMountedStatues().Num() > 0;
}

bool UPedastal::IsUsefulStatueMounted(TArray<int> StatueNumbers) // TODO rename to IsUsefulStatueMounted
{
	bool UsefulStatueMounted = false;

	for (const auto& Statue : GetMountedStatues()) {
		for (const int StatueNumber : StatueNumbers) {
			if (StatueNumber != CorrectStatueNumber && Statue->GetStatueNumber() == StatueNumber) {
				UsefulStatueMounted = true;
				break;
			}
		}
	}

	return UsefulStatueMounted;
}

bool UPedastal::IsCorrectStatueMounted()
{
	if (CorrectStatueNumber < 0) { return false; }

	bool IsCorrect = false;

	for (const auto& Statue : GetMountedStatues()) {
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

TArray<UStatue*> UPedastal::GetMountedStatues()
{
	TArray<UStatue*> Statues;

	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OUT OverlappingActors);
	for (const auto& Actor : OverlappingActors) {
		UStatue* Statue = Actor->FindComponentByClass<UStatue>();
		if (Statue) {
			Statues.Add(Statue);
		}
	}

	return Statues;
}


