// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawnVolume.h"

#include "Meteo.h"
#include "ObstaclePoolManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ufododgegame/gamestate/UFOGameState.h"

AObstacleSpawnVolume::AObstacleSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AObstacleSpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AObstacleSpawnVolume::SpawnObstacle,
		SpawnInterval,
		true
	);
}

void AObstacleSpawnVolume::SpawnObstacle()
{
	if (!bIsActivate) return;
	
	GS = GS == nullptr ? Cast<AUFOGameState>(UGameplayStatics::GetGameState(this)) : GS;
	if (!GS || !GS->ObstaclePoolManager) return;

	const FVector PlayerPos = GetPlayerLocation();

	switch (SpawnPattern)
	{
	case ESpawnPattern::ToPlayer:
		{
			FVector SpawnPos = GetRandomPointInVolume();
			DrawDebugSphere(GetWorld(), SpawnPos, 1.f, 8, FColor::Red);
			SpawnSingle(SpawnPos, PlayerPos);
		}
		break;

	case ESpawnPattern::ToTargetActor:
		{
			FVector SpawnPos = GetRandomPointInVolume();
			FVector TargetPos = TargetActor ? TargetActor->GetActorLocation() : PlayerPos;
			SpawnSingle(SpawnPos, TargetPos);
		}
		break;

	case ESpawnPattern::LineBurst:
		{
			FVector SpawnPos = GetRandomPointInVolume();
			FVector Direction = (PlayerPos - SpawnPos).GetSafeNormal();

			// 플레이어 방향에 수직인 벡터 계산 (XY 평면 기준)
			FVector Perp = bHorizontalLine
				? FVector(-Direction.Y, Direction.X, 0.f).GetSafeNormal()
				: FVector(0.f, 0.f, 1.f);

			float HalfSpan = LineBurstSpacing * (LineBurstCount - 1) * 0.5f;
			for (int32 i = 0; i < LineBurstCount; i++)
			{
				float Offset = -HalfSpan + LineBurstSpacing * i;
				FVector BurstPos = SpawnPos + Perp * Offset;
				SpawnSingle(BurstPos, PlayerPos);
			}
		}
		break;

	case ESpawnPattern::Homing:
		{
			FVector SpawnPos = GetRandomPointInVolume();
			SpawnSingle(SpawnPos, PlayerPos, true);
		}
		break;
	}
}

void AObstacleSpawnVolume::SpawnSingle(const FVector& SpawnPos, const FVector& TargetPos, bool bHoming)
{
	GS = GS == nullptr ? Cast<AUFOGameState>(UGameplayStatics::GetGameState(this)) : GS;

	if (!GS || !GS->ObstaclePoolManager) return;

	AProjectile* Obstacle = GS->ObstaclePoolManager->GetPooledObstacle();
	if (!Obstacle) return;

	// 플레이어 Z에 맞춰 고정
	FVector AdjustedSpawnPos = SpawnPos;
	AdjustedSpawnPos.Z = GetPlayerLocation().Z;

	FVector Direction = (TargetPos - AdjustedSpawnPos);
	Direction.Z = 0.f;
	Direction = Direction.GetSafeNormal();
	Obstacle->SetActorLocation(AdjustedSpawnPos);
	Obstacle->SetActorRotation(Direction.Rotation());

	if (bHoming)
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
		if (AMeteo* Meteo = Cast<AMeteo>(Obstacle))
		{
			if (Player)
			{
				Meteo->EnableHoming(Player->GetRootComponent());
			}
		}
	}

	Obstacle->StartProjectile();
}

FVector AObstacleSpawnVolume::GetRandomPointInVolume() const
{
	FVector Origin = SpawnBox->GetComponentLocation();
	FVector Extent = SpawnBox->GetScaledBoxExtent();
	
	return FVector(
		FMath::RandRange(Origin.X - Extent.X, Origin.X + Extent.X),
		FMath::RandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y),
		FMath::RandRange(Origin.Z - Extent.Z, Origin.Z + Extent.Z)
	);
}

FVector AObstacleSpawnVolume::GetPlayerLocation() const
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	return Player ? Player->GetActorLocation() : FVector::ZeroVector;
}
