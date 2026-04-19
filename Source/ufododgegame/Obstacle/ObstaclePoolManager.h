// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstaclePoolManager.generated.h"

UCLASS()
class UFODODGEGAME_API UObstaclePoolManager : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ObstacleClass;
	
	UPROPERTY(EditAnywhere)
	int32 PoolSize = 40;
	
	UPROPERTY(EditAnywhere)
	TArray<AProjectile*> ObstaclePool;
	
public:	
	// Sets default values for this actor's properties
	UObstaclePoolManager();

	void InitPool();
	AProjectile* GetPooledObstacle();
	void ReturnToPool(AProjectile* _Obstacle);
	void ExpandPool(int32 AdditionalCount, int32 MaxSize);
	int32 GetActiveCount() const;
	int32 GetPoolSize() const { return ObstaclePool.Num(); }
};
