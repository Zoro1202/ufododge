// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UFOPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UFODODGEGAME_API AUFOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TArray<class UInputMappingContext*> MappingContext;
	
	UPROPERTY(EditAnywhere)
	class AUFOHUD* HUD;
protected:
	virtual void BeginPlay() override;
	
	
	virtual void SetupInputComponent() override;
public:
	void SetHUDElapseddTime(float ElapsedTime);
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDMeteorCount(int32 Count);
	
};
