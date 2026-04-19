// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstaclePoolManager.h"

#include "Projectile.h"

// Sets default values
UObstaclePoolManager::UObstaclePoolManager()
{
	UE_LOG(LogTemp, Warning, TEXT("ObstaclePoolManager()"));
}

void UObstaclePoolManager::InitPool()
{
	if (!ObstacleClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObstaclePoolManager: ObstacleClass가 설정되지 않았습니다."));
		return;
	}

	ObstaclePool.Reserve(PoolSize);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < PoolSize; i++)
	{
		AProjectile* Obstacle = GetWorld()->SpawnActor<AProjectile>(
			ObstacleClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);

		if (Obstacle)
		{
			Obstacle->OwningPool = this;
			Obstacle->SetActorHiddenInGame(true);
			Obstacle->SetActorEnableCollision(false);
			Obstacle->SetActorTickEnabled(false);
			Obstacle->StopProjectile();

			ObstaclePool.Add(Obstacle);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ObstaclePoolManager: 풀 초기화 완료 (%d / %d)"), ObstaclePool.Num(), PoolSize);
}

AProjectile* UObstaclePoolManager::GetPooledObstacle()
{
	for (AProjectile* _Obstacle : ObstaclePool)
	{
		if (_Obstacle && _Obstacle->IsHidden())
		{
			_Obstacle->SetActorHiddenInGame(false);
			_Obstacle->SetActorEnableCollision(true);
			_Obstacle->SetActorTickEnabled(true);
			_Obstacle->StartProjectile();

			return _Obstacle;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ObstaclePoolManager: 풀 고갈! 현재 PoolSize = %d"), PoolSize);
	return nullptr;
}

void UObstaclePoolManager::ExpandPool(int32 AdditionalCount, int32 MaxSize)
{
	if (!ObstacleClass) return;

	int32 ToAdd = FMath::Min(AdditionalCount, MaxSize - ObstaclePool.Num());
	if (ToAdd <= 0) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < ToAdd; i++)
	{
		AProjectile* Obstacle = GetWorld()->SpawnActor<AProjectile>(
			ObstacleClass, FVector::ZeroVector, FRotator::ZeroRotator, Params
		);
		if (Obstacle)
		{
			Obstacle->OwningPool = this;
			Obstacle->SetActorHiddenInGame(true);
			Obstacle->SetActorEnableCollision(false);
			Obstacle->SetActorTickEnabled(false);
			Obstacle->StopProjectile();
			ObstaclePool.Add(Obstacle);
		}
	}

	PoolSize = ObstaclePool.Num();
	UE_LOG(LogTemp, Log, TEXT("ObstaclePoolManager: 풀 확장 → %d개"), PoolSize);
}

int32 UObstaclePoolManager::GetActiveCount() const
{
	int32 Count = 0;
	for (const AProjectile* Obstacle : ObstaclePool)
	{
		if (Obstacle && !Obstacle->IsHidden())
		{
			++Count;
		}
	}
	return Count;
}

void UObstaclePoolManager::ReturnToPool(AProjectile* _Obstacle)
{
	if (!_Obstacle)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObstaclePoolManager: nullptr 반환 시도"));
		return;
	}

	// 이 풀 소유 액터인지 확인
	if (!ObstaclePool.Contains(_Obstacle))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObstaclePoolManager: 이 풀 소유가 아닌 액터 반환 시도"));
		return;
	}

	_Obstacle->SetActorHiddenInGame(true);
	_Obstacle->SetActorEnableCollision(false);
	_Obstacle->SetActorTickEnabled(false);
	_Obstacle->StopProjectile();
	_Obstacle->SetActorLocation(FVector::ZeroVector);  // 월드 원점으로 리셋
}


