// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInstance.h"
#include "DoorLight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorLight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorLight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	enum DoorLightColor {
		Off, Amber, Green
	};

	void SetColor(DoorLightColor color);

private:
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* OffMaterial = nullptr;
	
	UPROPERTY(EditAnywhere)
		class UMaterialInstance* AmberMaterial = nullptr;

	UPROPERTY(EditAnywhere)
		class UMaterialInstance* GreenMaterial = nullptr;

	bool HaveMaterials = false;
};
