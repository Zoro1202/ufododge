// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "ufododgegame/character/UFOCharacter.h"

AHealthPickup::AHealthPickup()
{
}

void AHealthPickup::OnPickedUp(AUFOCharacter* Character)
{
	Character->HealHealth(HealAmount);
}
