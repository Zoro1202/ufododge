// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "ObstaclePoolManager.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	SetRootComponent(HitBox);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBox->SetCollisionObjectType(ECC_WorldDynamic);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->SetGenerateOverlapEvents(true);

	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOutOfBounds()
{
	if (OwningPool)
	{
		OwningPool->ReturnToPool(this);
	}
}

void AProjectile::StopProjectile()
{
	GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);
	ProjectileMovementComponent->StopMovementImmediately();
}

void AProjectile::StartProjectile()
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * DefaultSpeed);
	GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &AProjectile::OnOutOfBounds, Lifetime, false);
}