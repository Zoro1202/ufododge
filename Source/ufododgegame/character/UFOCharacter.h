// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "UFOCharacter.generated.h"

UCLASS()
class UFODODGEGAME_API AUFOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUFOCharacter();
	
	
private:
	/**
	 * Character 
	 */
	UPROPERTY(EditAnywhere, Category = "Player")
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Player")
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = "Player")
	float Health = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Player")
	float MaxHealth = 100.f;
	
	/**
	* Input
	*/
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* UseAction;

	/**
	 * Niagara
	 */
	

	
	void MoveInput(const FInputActionValue& InputActionValue);
	void LookInput(const FInputActionValue& InputActionValue);
	void MouseInput();
	void DoMove(float Right, float Forward);
	void UseInputPressed();
	void UseInputReleased();
	
	void Fire(bool Triggered);
	void SpawnBullet();

	FTimerHandle FireTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.15f;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float BulletDamage = 10.f;
protected:
	void UpdateHUDHealth();
	void PlayHitReaction();
	void Eliminated(const AController* InstigatorController);
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HealHealth(float Amount);
	void BoostBulletDamage(float Amount);

};
