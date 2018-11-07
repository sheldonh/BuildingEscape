// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorLight.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UDoorLight::UDoorLight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorLight::BeginPlay()
{
	Super::BeginPlay();
	
	if (!WrongMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing WrongMaterial on %s"), *GetOwner()->GetName());
	} else if (!UsefulMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing UsefulMaterial on %s"), *GetOwner()->GetName());
	} else if (!CorrectMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing CorrectMaterial on %s"), *GetOwner()->GetName());
	} else {
		HaveMaterials = true;
	}
	SetColor(Wrong);
}

// Called every frame
void UDoorLight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDoorLight::SetColor(DoorLightColor color)
{
	if (!HaveMaterials) { return; }

	UStaticMeshComponent* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>(); // TODO Move to member
	if (Mesh) {
		switch (color) {
		case Off:
			Mesh->SetMaterial(0, OffMaterial);
			break;
		case Wrong:
			Mesh->SetMaterial(0, WrongMaterial);
			break;
		case Useful:
			Mesh->SetMaterial(0, UsefulMaterial);
			break;
		case Correct:
			Mesh->SetMaterial(0, CorrectMaterial);
			break;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing Mesh on %s"), *GetOwner()->GetName());
	}

}

