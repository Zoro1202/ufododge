// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOPlayerController.h"

#include "CharacterOverlay.h"
#include "EnhancedInputSubsystems.h"
#include "UFOHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AUFOPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AUFOPlayerController::SetHUDElapseddTime(float ElapsedTime)
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	bool bHUDValid = HUD &&
		HUD->CharacterOverlay &&
		HUD->CharacterOverlay->ElapseTimeText;
	if (bHUDValid)
	{
		const uint8 Minute = static_cast<uint8>(ElapsedTime) / 60;
		const uint8 Second = static_cast<uint8>(ElapsedTime) % 60;

		FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minute, Second);
		HUD->CharacterOverlay->ElapseTimeText->SetText(FText::FromString(TimeText));
	}
}

void AUFOPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	bool bHUDValid = HUD &&
		HUD->CharacterOverlay &&
		HUD->CharacterOverlay->HealthProgressBar &&
		HUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		HUD->CharacterOverlay->HealthProgressBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(Health), FMath::RoundToInt(MaxHealth));
		HUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AUFOPlayerController::SetHUDMeteorCount(int32 Count)
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	bool bHUDValid = HUD &&
		HUD->CharacterOverlay &&
		HUD->CharacterOverlay->MeteorCount;
	if (bHUDValid)
	{
		FString CountText = FString::Printf(TEXT("운석: %d"), Count);
		HUD->CharacterOverlay->MeteorCount->SetText(FText::FromString(CountText));
	}
}

void AUFOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
		// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : MappingContext)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
