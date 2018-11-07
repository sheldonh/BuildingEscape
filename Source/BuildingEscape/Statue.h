// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Statue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatueEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UStatue : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatue();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Highlighted;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DisableHighlight();
	void EnableHighlight();

private:
	UPROPERTY(BlueprintAssignable)
		FStatueEvent OnEnableHighlight;

	UPROPERTY(BlueprintAssignable)
		FStatueEvent OnDisableHighlight;

};
