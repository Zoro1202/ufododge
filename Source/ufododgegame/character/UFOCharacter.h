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
	
	UPROPERTY(EditAnywhere, Category = "Player")
	float MaxYawRot = 30.f;
	UPROPERTY(EditAnywhere, Category = "Player")
	float MaxPitchRot = 30.f;
	
	float CurrentXSpeed = 0.f;
	float CurrentYSpeed = 0.f;
	
	/**
	 * Combat
	 */
	
	UPROPERTY(VisibleAnywhere, Category = "Player|Combat")
	float Health = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Player|Combat")
	float MaxHealth = 100.f;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void PlayHitReaction();
	void UpdateHUDHealth();
	void ToggleMenu();
	void Eliminated(const AController* InstigatorController);

	void Fire(bool Triggered);
	void SpawnBullet();

	FTimerHandle FireTimerHandle;
	
	UPROPERTY(VisibleAnywhere, Category = "Player|Combat")
	bool bCanShoot = true;
	
	UPROPERTY(EditAnywhere, Category = "Player|Combat")
	float FireRate = 0.15f;

	UPROPERTY(VisibleAnywhere, Category = "Player|Combat")
	float BulletDamage = 10.f;
	
	/**
	* Input
	*/
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	UInputAction* MouseAction;
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	UInputAction* UseAction;
	UPROPERTY(EditAnywhere, Category = "Player|Input")
	UInputAction* MenuAction;

	void MoveInput(const FInputActionValue& InputActionValue);
	void LookInput(const FInputActionValue& InputActionValue);
	void MouseInput();
	void DoMove(float Right, float Forward);
	void UseInputPressed();
	void UseInputReleased();
	
	/**
	 * Niagara
	 */

protected:
	
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
