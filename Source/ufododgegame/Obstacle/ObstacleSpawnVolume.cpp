// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawnVolume.h"

#include "Projectile.h"
#include "ObstaclePoolManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ufododgegame/gamestate/UFOGameState.h"

AObstacleSpawnVolume::AObstacleSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;  // Tick 불필요

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
		true  // 반복
	);
}

void AObstacleSpawnVolume::SpawnObstacle()
{
	AUFOGameState* GS = Cast<AUFOGameState>(UGameplayStatics::GetGameState(this));
	if (!GS || !GS->ObstaclePoolManager) return;

	AProjectile* Obstacle = GS->ObstaclePoolManager->GetPooledObstacle();
	if (!Obstacle) return;

	FVector SpawnPos = GetRandomPointInVolume();
	FVector TargetPos = TargetActor ? TargetActor->GetActorLocation() : FVector::ZeroVector;
	FVector Direction = (TargetPos - SpawnPos).GetSafeNormal();

	Obstacle->SetActorLocation(SpawnPos);
	Obstacle->SetActorRotation(Direction.Rotation());
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