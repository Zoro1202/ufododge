// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

UCLASS()
class UFODODGEGAME_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	AHealthPickup();

protected:
	virtual void OnPickedUp(class AUFOCharacter* Character) override;

private:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float HealAmount = 50.f;
};
