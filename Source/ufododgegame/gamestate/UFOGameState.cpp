// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOGameState.h"

#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "ufododgegame/character/UFOCharacter.h"
#include "ufododgegame/Obstacle/ObstaclePoolManager.h"
#include "ufododgegame/Obstacle/ObstacleSpawnVolume.h"
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
	
	PC = Cast<AUFOPlayerController>(GetWorld()->GetFirstPlayerController());
	
	ObstaclePoolManager->InitPool();
	BulletPoolManager->InitPool();
	
	PlayIntroSequence();
	
}

void AUFOGameState::PlayIntroSequence()
{
	if (!IntroSequence) return;

	// 시퀀스 시작 전 게임 비활성화
	SetGameActive(false);

	// 종료 콜백 바인딩
	ALevelSequenceActor* OutActor = nullptr;
	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(), IntroSequence, FMovieSceneSequencePlaybackSettings(), OutActor
		);;
	if (Player)
	{
		Player->OnFinished.AddDynamic(this, &AUFOGameState::OnIntroSequenceFinished);
		Player->Play();
	}
}

void AUFOGameState::OnIntroSequenceFinished()
{
	// 시퀀스 끝나면 게임 활성화
	SetGameActive(true);
}

void AUFOGameState::SetGameActive(bool bActive)
{
	bGameActive = bActive;
	
	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(this, AObstacleSpawnVolume::StaticClass(), SpawnVolumes);
	for (AActor* Volume : SpawnVolumes)
	{
		Cast<AObstacleSpawnVolume>(Volume)->SetSpawnActive(bActive);
	}

	PC = PC == nullptr ? Cast<AUFOPlayerController>(GetWorld()->GetFirstPlayerController()) : PC;
	if (PC)
	{
		AUFOCharacter* Player = Cast<AUFOCharacter>(PC->GetPawn());
		if (Player)
		{
			bActive ? Player->EnableInput(PC) : Player->DisableInput(PC);
		}
	}
	// 풀 성장 타이머도 같이 제어
	if (bActive)
	{
		GetWorldTimerManager().SetTimer(
			PoolGrowthTimerHandle, this,
			&AUFOGameState::ExpandObstaclePool,
			PoolGrowthInterval, true
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(PoolGrowthTimerHandle);
	}
}

void AUFOGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameOver && bGameActive)
	{
		SurvivalTime += DeltaTime;
	}

	PC = PC == nullptr ? Cast<AUFOPlayerController>(GetWorld()->GetFirstPlayerController()) : PC;
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

	SetGameActive(false);  // 스폰 중단 + 입력 차단

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
	if (PC)
	{
		PC->ShowDeathScreen(SurvivalTime);
	}

	UGameplayStatics::SetGamePaused(this, true);
}
