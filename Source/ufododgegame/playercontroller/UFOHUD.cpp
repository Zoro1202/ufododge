// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOHUD.h"

#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"

void AUFOHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AddCharacterOverlay();
}

void AUFOHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}
