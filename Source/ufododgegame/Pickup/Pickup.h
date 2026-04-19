// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class UFODODGEGAME_API APickup : public AActor
{
	GENERATED_BODY()

public:
	APickup();

protected:
	virtual void BeginPlay() override;
	virtual void OnPickedUp(class AUFOCharacter* Character) {}

private:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	class USphereComponent* SphereCollision;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
