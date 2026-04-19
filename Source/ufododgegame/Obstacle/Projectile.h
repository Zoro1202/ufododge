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
	UPROPERTY(EditAnywhere)
	class UBoxComponent* HitBox;
	
public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditAnywhere)
	float DefaultSpeed = 500.f;
	float Damage = 10.f;
protected:
	// UFUNCTION()
	// void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnOutOfBounds();
	void StopProjectile();
	void StartProjectile();
	
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
};
