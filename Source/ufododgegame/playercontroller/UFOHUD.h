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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlayReady);

	UPROPERTY()
	FOnOverlayReady OnOverlayReady;
	
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> MenuOverlayClass;
	
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
	UPROPERTY()
	class UMenuOverlay* MenuOverlay;
	
protected:
	virtual void BeginPlay() override;
	
	void AddCharacterOverlay();
};
