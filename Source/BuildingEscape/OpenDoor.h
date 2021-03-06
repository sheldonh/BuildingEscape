// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorLight.h"
#include "Engine/TriggerVolume.h"
#include "Pedastal.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorLightEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
private:
	UPROPERTY(EditAnywhere)
		TArray<AActor*> PedastalActors = TArray<AActor*>();

	UPROPERTY(EditAnywhere)
		TArray<AActor*> DoorLightActors = TArray<AActor*>();

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

	UPROPERTY(BlueprintAssignable)
		FDoorLightEvent OnRight;

	UPROPERTY(BlueprintAssignable)
		FDoorLightEvent OnWrong;

	TArray<UPedastal*> Pedastals;
	TArray<UDoorLight*> DoorLights;

	bool AreAllPedastalsCorrectlyMounted();
	void SetLightColors();

	int LastStatuesMounted = -1;

	struct MountingAttempt {
		int Correct;
		int Useful;
		bool IsUnlocked;
	};

	MountingAttempt LastMountingAttempt = { -1, -1, false };
};
