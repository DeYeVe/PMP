// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PMPUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API UPMPUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurHPText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHPText;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* AuroraSkill;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MurielSkill;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AuroraSkill1;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AuroraSkill2;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AuroraSkill3;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MurielSkill1;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MurielSkill2;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MurielSkill3;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* BoostPanel;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Boost;

	UPROPERTY(meta = (BindWidget))
	class UImage* BoostBG;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* StrengthenPanel;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Strengthen;

	UPROPERTY(meta = (BindWidget))
	class UImage* StrengthenBG;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* InvinciblePanel;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Invincible;

	UPROPERTY(meta = (BindWidget))
	class UImage* InvincibleBG;
};
