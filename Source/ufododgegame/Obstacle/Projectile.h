// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class UFODODGEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* HitBox;

public:
	AProjectile();

	UPROPERTY(EditAnywhere)
	float DefaultSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY()
	class UObstaclePoolManager* OwningPool = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnOutOfBounds();
	virtual void StopProjectile();
	void StartProjectile();

	UPROPERTY(EditAnywhere)
	float Lifetime = 5.f;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

private:
	FTimerHandle LifetimeTimerHandle;
};
