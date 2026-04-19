// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "UFOGameState.generated.h"

/**
 * 
 */
UCLASS()
class UFODODGEGAME_API AUFOGameState : public AGameState
{
	GENERATED_BODY()
	
	AUFOGameState();
public:
	UPROPERTY(VisibleAnywhere)
	class UObstaclePoolManager* ObstaclePoolManager;
	
protected:
	virtual void BeginPlay() override;
	
};
