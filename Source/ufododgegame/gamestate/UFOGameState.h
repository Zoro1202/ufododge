// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "UFOGameState.generated.h"

UCLASS()
class UFODODGEGAME_API AUFOGameState : public AGameState
{
	GENERATED_BODY()

	AUFOGameState();

public:
	UPROPERTY(VisibleAnywhere)
	class UObstaclePoolManager* ObstaclePoolManager;

	UPROPERTY(VisibleAnywhere)
	class UObstaclePoolManager* BulletPoolManager;

	UPROPERTY(EditAnywhere, Category = "Game")
	class ULevelSequence* DeathSequence;

	UPROPERTY(VisibleAnywhere, Category = "Game")
	float SurvivalTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Pool Growth")
	float PoolGrowthInterval = 30.f;

	UPROPERTY(EditAnywhere, Category = "Pool Growth")
	int32 PoolGrowthAmount = 20;

	UPROPERTY(EditAnywhere, Category = "Pool Growth")
	int32 MaxPoolSize = 100;

	void OnPlayerEliminated();
	float GetSurvivalTime() const { return SurvivalTime; }

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class ULevelSequence* IntroSequence;
	
protected:
	virtual void BeginPlay() override;
	void PlayIntroSequence();
	UFUNCTION()
	void OnDeathSequenceFinished();
	UFUNCTION()
	void OnIntroSequenceFinished();
	void SetGameActive(bool bActive);
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class AUFOPlayerController* PC;
	
	bool bGameOver = false;
	
    bool bGameActive = false;

	FTimerHandle PoolGrowthTimerHandle;

	void ExpandObstaclePool();

	
};
