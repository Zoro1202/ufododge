// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UFOHUD.generated.h"

/**
 * 
 */
UCLASS()
class UFODODGEGAME_API AUFOHUD : public AHUD
{
	GENERATED_BODY()
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
protected:
	virtual void BeginPlay() override;
	
	void AddCharacterOverlay();
public:
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
};
