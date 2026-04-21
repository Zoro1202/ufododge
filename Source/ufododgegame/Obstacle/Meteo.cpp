// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteo.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerBullet.h"

AMeteo::AMeteo()
{
	HitBox->SetCollisionObjectType(ECC_GameTraceChannel2);
	HitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitEffect = CreateDefaultSubobject<UNiagaraSystem>(TEXT("HitParticle"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FX(TEXT("/Game/MsvFx_Niagara_Explosion_Pack_01/Prefabs/Niagara_Explosion_08"));
	if (FX.Succeeded()) HitEffect = FX.Object;
	
}

void AMeteo::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeteo::OnBeginOverlap);
	StopProjectile();
}

void AMeteo::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	float RotSpeed = 90.f;
	FQuat DeltaRot = FQuat(FVector(1, 1, 1).GetSafeNormal(), FMath::DegreesToRadians(RotSpeed * DeltaSeconds));
	StaticMesh->AddRelativeRotation(DeltaRot);
}

void AMeteo::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || Cast<AMeteo>(OtherActor)) return;

	if (APlayerBullet* Bullet = Cast<APlayerBullet>(OtherActor))
	{
		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect,
				GetActorLocation(), FRotator(90.f, 0.f, 0.f), FVector(0.01f));
		}
		TrySpawnPickup();
		Bullet->OnOutOfBounds();
		OnOutOfBounds();
		return;
	}
	if (HitEffect)
	{
		FVector ImpactPoint = GetActorLocation();
		FRotator SpawnRot = FRotator(90.f, 0.f, 0.f);
		if (UWorld* World = GetWorld())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitEffect, ImpactPoint, SpawnRot, FVector(0.01f));
		}
	}

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);

	TrySpawnPickup();
	OnOutOfBounds();
}

void AMeteo::TrySpawnPickup()
{
	if (!PickupClass) return;
	if (FMath::RandRange(0.f, 1.f) > PickupSpawnChance) return;

	GetWorld()->SpawnActor<AActor>(PickupClass, GetActorLocation(), FRotator::ZeroRotator);
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
