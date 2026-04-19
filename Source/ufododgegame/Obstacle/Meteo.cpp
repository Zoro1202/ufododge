// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteo.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AMeteo::AMeteo()
{
}

void AMeteo::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeteo::OnBeginOverlap);
}

void AMeteo::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || Cast<AProjectile>(OtherActor)) return;

	if (HitEffect)
	{
		FRotator HitRotation = GetActorRotation();
		HitRotation.Pitch -= 90.f;

		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), HitEffect, GetActorLocation(), HitRotation
		);
		if (NiagaraComp)
		{
			NiagaraComp->SetVariableFloat(FName("Explosion_Rate"), 1.f);
			
		}
	}

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	OnOutOfBounds();
}

void AMeteo::EnableHoming(USceneComponent* TargetComponent)
{
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAcceleration;
	ProjectileMovementComponent->HomingTargetComponent = TargetComponent;
}

void AMeteo::StopProjectile()
{
	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	Super::StopProjectile();
}
