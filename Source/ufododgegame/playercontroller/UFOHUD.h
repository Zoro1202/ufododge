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
public:

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
protected:
	virtual void BeginPlay() override;
	
	void AddCharacterOverlay();
};
