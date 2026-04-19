// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOGameState.h"

#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "ufododgegame/Obstacle/ObstaclePoolManager.h"
#include "ufododgegame/playercontroller/UFOPlayerController.h"

AUFOGameState::AUFOGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	ObstaclePoolManager = CreateDefaultSubobject<UObstaclePoolManager>(TEXT("ObstaclePoolManager"));
	BulletPoolManager = CreateDefaultSubobject<UObstaclePoolManager>(TEXT("BulletPoolManager"));
}

void AUFOGameState::BeginPlay()
{
	Super::BeginPlay();
	ObstaclePoolManager->InitPool();
	BulletPoolManager->InitPool();

	GetWorldTimerManager().SetTimer(
		PoolGrowthTimerHandle,
		this,
		&AUFOGameState::ExpandObstaclePool,
		PoolGrowthInterval,
		true
	);
}

void AUFOGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameOver)
	{
		SurvivalTime += DeltaTime;
	}

	AUFOPlayerController* PC = Cast<AUFOPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->SetHUDElapseddTime(SurvivalTime);
		if (ObstaclePoolManager)
		{
			PC->SetHUDMeteorCount(ObstaclePoolManager->GetActiveCount());
		}
	}
}

void AUFOGameState::OnPlayerEliminated()
{
	if (bGameOver) return;
	bGameOver = true;

	if (DeathSequence)
	{
		ALevelSequenceActor* OutActor = nullptr;
		ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(), DeathSequence, FMovieSceneSequencePlaybackSettings(), OutActor
		);
		if (Player)
		{
			Player->OnFinished.AddDynamic(this, &AUFOGameState::OnDeathSequenceFinished);
			Player->Play();
			return;
		}
	}

	// 시퀀스 없으면 바로 정지
	OnDeathSequenceFinished();
}

void AUFOGameState::ExpandObstaclePool()
{
	if (ObstaclePoolManager)
	{
		ObstaclePoolManager->ExpandPool(PoolGrowthAmount, MaxPoolSize);
	}
}

void AUFOGameState::OnDeathSequenceFinished()
{
	UGameplayStatics::SetGamePaused(this, true);
}
