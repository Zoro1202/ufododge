// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Meteo.generated.h"

UCLASS()
class UFODODGEGAME_API AMeteo : public AProjectile
{
	GENERATED_BODY()

public:
	AMeteo();

	void EnableHoming(USceneComponent* TargetComponent);
	virtual void StopProjectile() override;

	UPROPERTY(EditAnywhere, Category = "Homing")
	float HomingAcceleration = 800.f;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "FX")
	float HitEffectDuration = 2.f;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
