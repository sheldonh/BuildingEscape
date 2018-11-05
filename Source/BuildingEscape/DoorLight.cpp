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
	
	if (!OffMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing OffMaterial on %s"), *GetOwner()->GetName());
	} else if (!AmberMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing AmberMaterial on %s"), *GetOwner()->GetName());
	} else if (!GreenMaterial) {
		UE_LOG(LogTemp, Error, TEXT("Missing GreenMaterial on %s"), *GetOwner()->GetName());
	} else {
		HaveMaterials = true;
	}
	SetColor(Off);
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
		case Amber:
			Mesh->SetMaterial(0, AmberMaterial);
			break;
		case Green:
			Mesh->SetMaterial(0, GreenMaterial);
			break;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing Mesh on %s"), *GetOwner()->GetName());
	}

}

