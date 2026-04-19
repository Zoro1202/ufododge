// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "ObstaclePoolManager.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/GameState.h"
#include "ufododgegame/gamestate/UFOGameState.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	SetRootComponent(HitBox);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // Query Only
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->SetGenerateOverlapEvents(true);
	
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.f,
			FColor::Red,
			FString("Obstacle: BeginPlay"));
	}
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.f,
			FColor::Red,
			FString("Obstacle: OnHit"));
	}
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	OnOutOfBounds();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnOutOfBounds()
{
	UWorld* World = GetWorld();
	if (World)
	{
		class AUFOGameState* GameState =  Cast<AUFOGameState>(World->GetGameState());
		if (GameState)
		{
			GameState->ObstaclePoolManager->ReturnToPool(this);
		}
	}
}

void AProjectile::StopProjectile()
{
	ProjectileMovementComponent->StopMovementImmediately();
}

void AProjectile::StartProjectile()
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * DefaultSpeed);
}
