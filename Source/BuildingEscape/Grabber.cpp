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
		PhysicsHandle->SetTargetLocationAndRotation(GetReachLineEnd(), GetGrabRotation());
	} else {
		UStatue* Statue = GetFirstStatueInReach();
		if (Statue) {
			if (!HighlightedStatue) {
				Statue->EnableHighlight();
				HighlightedStatue = Statue;
			} else if (Statue != HighlightedStatue) {
				HighlightedStatue->DisableHighlight();
				Statue->EnableHighlight();
				HighlightedStatue = Statue;
			}
		} else if (HighlightedStatue) {
			HighlightedStatue->DisableHighlight();
			HighlightedStatue = nullptr;
		}
	}
}

UStatue* UGrabber::GetFirstStatueInReach()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		return ActorHit->FindComponentByClass<UStatue>();
	}
	else {
		return nullptr;
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

FRotator UGrabber::GetGrabRotation()
{
	return FRotator(0.0, GetPlayerViewPointRotation().Yaw, 0.0);
}

void UGrabber::Grab()
{
	if (!PhysicsHandle) { return; }

	FHitResult Hit = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		GrabbedComponent = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			GrabbedComponent,
			NAME_None,
			GrabbedComponent->GetOwner()->GetActorLocation(),
			GetGrabRotation());
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

	if (PhysicsHandle->GrabbedComponent) {
		auto* GrabbedComponent = PhysicsHandle->GrabbedComponent;
		GrabbedComponent->SetAllPhysicsAngularVelocity(FVector::ZeroVector);
		GrabbedComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	}
	PhysicsHandle->ReleaseComponent();
	GrabbedComponent = nullptr;
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		Input->BindAction("Grab", IE_Released, this, &UGrabber::OnGrabKeyReleased);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing Input on %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::OnGrabKeyReleased()
{
	if (GrabbedComponent) {
		Release();
	} else {
		Grab();
	}
}
