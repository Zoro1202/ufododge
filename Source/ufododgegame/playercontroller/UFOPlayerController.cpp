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
	UE_LOG(LogTemp, Warning, TEXT("HUD: %s"), HUD ? TEXT("Valid") : TEXT("nullptr"));
	UE_LOG(LogTemp, Warning, TEXT("MenuOverlay: %s"), 
		(HUD && HUD->MenuOverlay) ? TEXT("Valid") : TEXT("nullptr"));
	
	// MenuOverlay가 이미 있으면 바로 바인딩
	if (HUD->MenuOverlay)
	{
		BindMenuButtons();
		return;
	}

	// 없으면 준비될 때까지 대기
	HUD->OnOverlayReady.AddDynamic(this, &AUFOPlayerController::BindMenuButtons);
}

void AUFOPlayerController::BindMenuButtons()
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	if (!HUD || !HUD->MenuOverlay) return;

	HUD->MenuOverlay->RestartButton->OnReleased.AddDynamic(this, &AUFOPlayerController::OnRestartClicked);
	HUD->MenuOverlay->MainButton->OnReleased.AddDynamic(this, &AUFOPlayerController::OnMainClicked);
	HUD->MenuOverlay->ResumeButton->OnReleased.AddDynamic(this, &AUFOPlayerController::OnResumeClicked);

	UE_LOG(LogTemp, Warning, TEXT("MenuButtons Bound"));
}

void AUFOPlayerController::ShowDeathScreen(float Score)
{
	HUD = HUD == nullptr ? Cast<AUFOHUD>(GetHUD()) : HUD;
	if (!HUD || !HUD->MenuOverlay) return;

	HUD->MenuOverlay->MenuText->SetText(FText::FromString(TEXT("RE?")));
	HUD->MenuOverlay->ScoreText->SetText(
		FText::FromString(FString::Printf(TEXT("기록 : %.2fs"), Score))
	);
	HUD->MenuOverlay->ResumeButton->SetVisibility(ESlateVisibility::Collapsed);
	HUD->MenuOverlay->SetVisibility(ESlateVisibility::Visible);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
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
	
	HUD->MenuOverlay->MenuText->SetText(FText::FromString(TEXT("MENU")));
	HUD->MenuOverlay->ResumeButton->SetVisibility(ESlateVisibility::Visible);
	HUD->MenuOverlay->ScoreText->SetVisibility(ESlateVisibility::Collapsed);
	
	bMenuActivate = !bMenuActivate;
	
	if (bMenuActivate)
	{
		HUD->MenuOverlay->SetVisibility(ESlateVisibility::Visible);
        
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
		// SetPause(true);
		UGameplayStatics::SetGlobalTimeDilation(this, 0.0001f);
	}
	else
	{
		HUD->MenuOverlay->SetVisibility(ESlateVisibility::Collapsed);
		
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
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
	UE_LOG(LogTemp, Warning, TEXT("OnRestartClicked"));
    ClientTravel("/Game/Asset/TreatmentStation/Map/TreatmentStationMap", TRAVEL_Absolute);
}

void AUFOPlayerController::OnMainClicked()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
    ClientTravel("/Game/Asset/TreatmentStation/Map/TreatmentStationMap", TRAVEL_Absolute);
}
