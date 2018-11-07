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

}

// Called every frame
void UStatue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatue::DisableHighlight()
{
	UE_LOG(LogTemp, Warning, TEXT("Disabling highlight on %s"), *GetOwner()->GetName());
	OnDisableHighlight.Broadcast();
}

void UStatue::EnableHighlight()
{
	UE_LOG(LogTemp, Warning, TEXT("Enabling highlight on %s"), *GetOwner()->GetName());
	OnEnableHighlight.Broadcast();
}

