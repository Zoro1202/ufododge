// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupPickup.h"

#include "ufododgegame/character/UFOCharacter.h"

APowerupPickup::APowerupPickup()
{
}

void APowerupPickup::OnPickedUp(AUFOCharacter* Character)
{
	Character->BoostBulletDamage(DamageBoost);
}
