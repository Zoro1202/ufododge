// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOGameState.h"

#include "ufododgegame/Obstacle/ObstaclePoolManager.h"

AUFOGameState::AUFOGameState()
{
	ObstaclePoolManager = CreateDefaultSubobject<UObstaclePoolManager>(TEXT("ObstaclePoolManager"));
}

void AUFOGameState::BeginPlay()
{
	Super::BeginPlay();
	ObstaclePoolManager->InitPool();
}
