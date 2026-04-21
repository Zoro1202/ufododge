// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Projectile.h"
#include "PlayerBullet.generated.h"

/**
 * 
 */
UCLASS()
class UFODODGEGAME_API APlayerBullet : public AProjectile
{
	GENERATED_BODY()
	
	APlayerBullet();
protected:
	virtual auto BeginPlay() -> void override;
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void StopProjectile() override;
	UPROPERTY()
	UNiagaraSystem* HitEffect;
};
