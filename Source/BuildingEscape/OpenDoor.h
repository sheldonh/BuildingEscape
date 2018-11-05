// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorLight.h"
#include "Engine/TriggerVolume.h"
#include "Pedastal.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

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
		TArray<AActor*> PedastalActors;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> DoorLightActors;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

	TArray<UPedastal*> Pedastals;
	TArray<UDoorLight*> DoorLights;

	bool AreAllPedastalsCorrectlyMounted();
	void SetLightColors();
};
