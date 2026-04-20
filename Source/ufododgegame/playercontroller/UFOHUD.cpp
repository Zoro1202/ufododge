// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOHUD.h"

#include "CharacterOverlay.h"
#include "MenuOverlay.h"
#include "Blueprint/UserWidget.h"

void AUFOHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AddCharacterOverlay();
}

void AUFOHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;
	if (CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
	if (MenuOverlayClass)
	{
		MenuOverlay = CreateWidget<UMenuOverlay>(PlayerController, MenuOverlayClass);
		MenuOverlay->AddToViewport(1);
		MenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
