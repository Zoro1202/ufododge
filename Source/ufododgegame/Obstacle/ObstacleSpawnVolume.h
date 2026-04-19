// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleSpawnVolume.generated.h"

UENUM(BlueprintType)
enum class ESpawnPattern : uint8
{
	ToPlayer      UMETA(DisplayName = "To Player"),
	ToTargetActor UMETA(DisplayName = "To Target Actor"),
	LineBurst     UMETA(DisplayName = "Line Burst"),
	Homing        UMETA(DisplayName = "Homing"),
};

UCLASS()
class UFODODGEGAME_API AObstacleSpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawnVolume();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval = 2.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	ESpawnPattern SpawnPattern = ESpawnPattern::ToPlayer;

	// ToTargetActor 패턴 전용
	UPROPERTY(EditAnywhere, Category = "Spawn")
	AActor* TargetActor;

	// LineBurst 패턴 전용
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bHorizontalLine = true;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 LineBurstCount = 5;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float LineBurstSpacing = 200.f;

	FTimerHandle SpawnTimerHandle;

	void SpawnObstacle();
	void SpawnSingle(const FVector& SpawnPos, const FVector& TargetPos, bool bHoming = false);

	FVector GetRandomPointInVolume() const;
	FVector GetPlayerLocation() const;
};
