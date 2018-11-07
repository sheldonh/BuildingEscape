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

	if (!CorrectStatueActor) {
		UE_LOG(LogTemp, Error, TEXT("Missing CorrectStatueActor on %s"), *GetOwner()->GetName());
	} else {
		CorrectStatue = CorrectStatueActor->FindComponentByClass<UStatue>();
		if (!CorrectStatue) {
			UE_LOG(LogTemp, Error, TEXT("Missing Statue component in %s on %s"), *CorrectStatueActor->GetName(), *GetOwner()->GetName());
		}
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

bool UPedastal::IsUsefulStatueMounted(TArray<UStatue*> UsefulStatues)
{
	if (!CorrectStatue) { return false; }

	bool UsefulStatueMounted = false;

	for (const auto& MountedStatue : GetMountedStatues()) {
		for (const auto& UsefulStatue : UsefulStatues) {
			if (MountedStatue == UsefulStatue) {
				UsefulStatueMounted = true;
				break;
			}
		}
	}

	return UsefulStatueMounted;
}

bool UPedastal::IsCorrectStatueMounted()
{
	if (!CorrectStatue) { return false; }

	bool IsCorrect = false;

	for (const auto& Statue : GetMountedStatues()) {
		if (Statue == CorrectStatue) {
			IsCorrect = true;
			break;
		}
	}

	return IsCorrect;
}

UStatue* UPedastal::GetCorrectStatue()
{
	return CorrectStatue;
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


