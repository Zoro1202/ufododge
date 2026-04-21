// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

APlayerBullet::APlayerBullet()
{
	HitBox->SetCollisionObjectType(ECC_GameTraceChannel1);  // Bullet 채널
	HitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	HitEffect = CreateDefaultSubobject<UNiagaraSystem>(TEXT("HitParticle"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FX(TEXT("/Game/MsvFx_Niagara_Explosion_Pack_01/Prefabs/Niagara_Explosion_08"));
	if (FX.Succeeded()) HitEffect = FX.Object;
}

void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::OnBeginOverlap);
}

void APlayerBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || Cast<APlayerBullet>(OtherActor)) return;
	
	if (HitEffect)
	{
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
void APlayerBullet::StopProjectile()
{
	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	Super::StopProjectile();
}
