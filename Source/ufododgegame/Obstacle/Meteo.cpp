// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteo.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"

AMeteo::AMeteo()
{
	
	HitEffect = CreateDefaultSubobject<UNiagaraSystem>(TEXT("HitParticle"));
	// /Game/MsvFx_Niagara_Explosion_Pack_01/Prefabs/Niagara_Explosion_02.Niagara_Explosion_03
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
	if (!OtherActor || Cast<AProjectile>(OtherActor)) return;

	if (HitEffect)
	{
		GEngine->AddOnScreenDebugMessage(
			2,
			1.f,
			FColor::Red,
			FString::Printf(TEXT("%d"), HitEffect->IsLooping())
		);
		FVector ImpactPoint = GetActorLocation();
		FRotator SpawnRot = FRotator(90.f, 0.f, 0.f);
		UWorld* World = GetWorld();
		if (World)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitEffect, ImpactPoint, SpawnRot, FVector(0.01f));
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
