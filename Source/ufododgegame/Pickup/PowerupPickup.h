// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PowerupPickup.generated.h"

UCLASS()
class UFODODGEGAME_API APowerupPickup : public APickup
{
	GENERATED_BODY()

public:
	APowerupPickup();

protected:
	virtual void OnPickedUp(class AUFOCharacter* Character) override;

private:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float DamageBoost = 10.f;
};
