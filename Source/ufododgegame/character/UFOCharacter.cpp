// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ufododgegame/playercontroller/UFOPlayerController.h"

AUFOCharacter::AUFOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	
}

void AUFOCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AUFOCharacter::ReceiveDamage);
	if (GetController())
	{
		AddControllerPitchInput(-90.f);
	}
}

void AUFOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		
		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		
		// Interaction 
		EnhancedInputComponent->BindAction(MouseAction, ETriggerEvent::Triggered, this, &ThisClass::MouseInput);
		
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ThisClass::UseInputPressed);
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Completed, this, &ThisClass::UseInputReleased);
	}
}

void AUFOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AUFOCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
	
}

void AUFOCharacter::LookInput(const FInputActionValue& Value)
{
	// 마우스 look 처리
	
}

void AUFOCharacter::MouseInput()
{
	// 마우스 클릭 처리
	
}

void AUFOCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AUFOCharacter::UseInputPressed()
{
	Fire(true);
}

void AUFOCharacter::UseInputReleased()
{
	Fire(false);
}

void AUFOCharacter::Fire(bool Triggered)
{
	
}

void AUFOCharacter::UpdateHUDHealth()
{
	// PlayerController에서 HUD 가져오기
	AUFOPlayerController* PC = Cast<AUFOPlayerController>(GetController());
	if (!PC) return;
	
	// WBP에 값 전달
	PC->SetHUDHealth(Health, MaxHealth);
}

void AUFOCharacter::PlayHitReaction()
{
	
}

void AUFOCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                  class AController* InstigatedBy, AActor* DamageCauser)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			0,
			1.f,
			FColor::Red,
			FString("UFOCharacter: ReceiveDamage"));
	}
	
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	
	UpdateHUDHealth();
	PlayHitReaction();
	
	if (Health == 0.f)
	{
		Eliminated(InstigatedBy);
	}
}

void AUFOCharacter::Eliminated(const AController* InstigatorController)
{
	if (InstigatorController)
		UE_LOG(LogTemp, Display, TEXT("Elim : Instigated by %s"), *InstigatorController->GetName());
}
