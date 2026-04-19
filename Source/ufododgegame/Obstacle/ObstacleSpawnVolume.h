// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleSpawnVolume.generated.h"

UCLASS()
class UFODODGEGAME_API AObstacleSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval = 2.f;

	// 스폰된 Obstacle이 향할 목표 (보통 플레이 영역 중심)
	UPROPERTY(EditAnywhere, Category = "Spawn")
	AActor* TargetActor;

	FTimerHandle SpawnTimerHandle;
	
	void SpawnObstacle();
	
	FVector GetRandomPointInVolume() const;
};
