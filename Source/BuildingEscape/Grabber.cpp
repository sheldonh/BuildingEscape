// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	FindPlayerController();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocationAndRotation(GetReachLineEnd(), GetPlayerViewPointRotation());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Missing PhysicsHandle on %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::FindPlayerController()
{
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) {
		UE_LOG(LogTemp, Error, TEXT("Missing PlayerController on World"));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	return Hit;
}

void UGrabber::Grab()
{
	if (!PhysicsHandle) { return; }

	FHitResult Hit = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			GetPlayerViewPointRotation());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	return GetPlayerViewPointLocation() + GetPlayerViewPointRotation().Vector() * Reach;
}

FVector UGrabber::GetReachLineStart()
{
	return GetPlayerViewPointLocation();
}

void UGrabber::UpdatePlayerViewPoint()
{
	if (!PlayerController) { return; }

	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
}

FVector UGrabber::GetPlayerViewPointLocation()
{
	UpdatePlayerViewPoint();

	return PlayerViewPointLocation;
}

FRotator UGrabber::GetPlayerViewPointRotation()
{
	UpdatePlayerViewPoint();

	return PlayerViewPointRotation;
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing Input on %s"), *(GetOwner()->GetName()));
	}
}
