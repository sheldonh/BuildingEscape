// Fill out your copyright notice in the Description page of Project Settings.

#include "Statue.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UStatue::UStatue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatue::BeginPlay()
{
	Super::BeginPlay();

	if (StatueNumber < 0) {
		UE_LOG(LogTemp, Error, TEXT("Missing StatueNumber on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UStatue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UStatue::GetStatueNumber()
{
	return StatueNumber;
}

