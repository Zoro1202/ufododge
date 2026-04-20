// Fill out your copyright notice in the Description page of Project Settings.


#include "UFOPlayerController.h"

#include "CharacterOverlay.h"
#include "EnhancedInputSubsystems.h"
#include "MenuOverlay.h"
#include "UFOHUD.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void AUFOPlayerController::BeginPlay()
{
	Super::BeginPlay();
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	if (!HUD || !HUD->MenuOverlay) return;

	HUD->MenuOverlay->RestartButton->OnClicked.AddDynamic(this, &AUFOPlayerController::OnRestartClicked);
	HUD->MenuOverlay->MainButton->OnClicked.AddDynamic(this, &AUFOPlayerController::OnMainClicked);
	HUD->MenuOverlay->ResumeButton->OnClicked.AddDynamic(this, &AUFOPlayerController::OnResumeClicked);
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
		FString CountText = FString::Printf(TEXT("%d"), Count);
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

void AUFOPlayerController::ToggleMenu()
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	if (!HUD || !HUD->MenuOverlay) return;
	
	bMenuActivate = !bMenuActivate;
	
	if (bMenuActivate)
	{
		HUD->MenuOverlay->SetVisibility(ESlateVisibility::Visible);
        
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
		// SetPause(true);
		UGameplayStatics::SetGlobalTimeDilation(this, 0.0001f);
	}
	else
	{
		HUD->MenuOverlay->SetVisibility(ESlateVisibility::Hidden);
		
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		// SetPause(false);
		UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	}
}

void AUFOPlayerController::OnResumeClicked()
{
	ToggleMenu();
	UE_LOG(LogTemp, Warning, TEXT("OnResumeClicked"));
}

void AUFOPlayerController::OnRestartClicked()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("OnRestartClicked"));
    ClientTravel("/Game/Asset/TreatmentStation/Map/TreatmentStationMap", TRAVEL_Absolute);
}

void AUFOPlayerController::OnMainClicked()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
    ClientTravel("/Game/Asset/TreatmentStation/Map/TreatmentStationMap", TRAVEL_Absolute);
}
