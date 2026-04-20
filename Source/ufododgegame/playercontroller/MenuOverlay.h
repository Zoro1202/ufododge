// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuOverlay.generated.h"

/**
 * 
 */
UCLASS()
class UFODODGEGAME_API UMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MenuText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	class UButton* MainButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;
};
