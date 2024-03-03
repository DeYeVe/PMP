// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPPlayerController.h"

#include "PMPCharacter.h"
#include "PMPHUD.h"
#include "PMPUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void APMPPlayerController::SetHUDHP(int32 CurHP, int32 MaxHP)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		if(MaxHP != 0)
		{			
			const float HPPercent = float(CurHP) / float(MaxHP);
			PMPHUD->PlayerOverlay->HPBar->SetPercent(HPPercent);
		}
		PMPHUD->PlayerOverlay->CurHPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurHP)));
		PMPHUD->PlayerOverlay->MaxHPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxHP)));
	}
}

void APMPPlayerController::SetHUDCooldown1(float CurCooldown, float MaxCooldown)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		const float CoolDownPercent = CurCooldown / MaxCooldown;
		PMPHUD->PlayerOverlay->AuroraSkill1->SetPercent(CoolDownPercent);
		PMPHUD->PlayerOverlay->MurielSkill1->SetPercent(CoolDownPercent);
	}
}

void APMPPlayerController::SetHUDCooldown2(float CurCooldown, float MaxCooldown)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		const float CoolDownPercent = CurCooldown / MaxCooldown;
		PMPHUD->PlayerOverlay->AuroraSkill2->SetPercent(CoolDownPercent);
		PMPHUD->PlayerOverlay->MurielSkill2->SetPercent(CoolDownPercent);
	}
}

void APMPPlayerController::SetHUDCooldown3(float CurCooldown, float MaxCooldown)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		const float CoolDownPercent = CurCooldown / MaxCooldown;
		PMPHUD->PlayerOverlay->AuroraSkill3->SetPercent(CoolDownPercent);
		PMPHUD->PlayerOverlay->MurielSkill3->SetPercent(CoolDownPercent);
	}
}

void APMPPlayerController::SetHUDBuffBoost(float RemainingTime, float Duration)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		if (RemainingTime <= 0.f)
		{
			PMPHUD->PlayerOverlay->BoostPanel->SetVisibility(ESlateVisibility::Hidden);
			IsBoost = false;
			return;
		}
		
		PMPHUD->PlayerOverlay->BoostPanel->SetVisibility(ESlateVisibility::Visible);
		float ElapsedTime = Duration - RemainingTime;
		const float DurationPercent = ElapsedTime / Duration;
		PMPHUD->PlayerOverlay->Boost->SetPercent(DurationPercent);
		IsBoost = true;
	}
}

void APMPPlayerController::SetHUDBufStrengthen(float RemainingTime, float Duration)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		if (RemainingTime <= 0.f)
		{
			PMPHUD->PlayerOverlay->StrengthenPanel->SetVisibility(ESlateVisibility::Hidden);
			IsStrengthened = false;
			return;
		}
		
		PMPHUD->PlayerOverlay->StrengthenPanel->SetVisibility(ESlateVisibility::Visible);
		float ElapsedTime = Duration - RemainingTime;
		const float DurationPercent = ElapsedTime / Duration;
		PMPHUD->PlayerOverlay->Strengthen->SetPercent(DurationPercent);
		IsStrengthened = true;
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PMPHUD->PlayerOverlay->StrengthenPanel->Slot);
		if (CanvasSlot)
		{
			if (IsBoost)
			{
				CanvasSlot->SetPosition(FVector2D(70.f, 0.f));
			}
			else
			{
				CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
			}
		}
	}
}

void APMPPlayerController::SetHUDBuffInvincible(float RemainingTime, float Duration)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{		
		if (RemainingTime <= 0.f)
		{
			PMPHUD->PlayerOverlay->InvinciblePanel->SetVisibility(ESlateVisibility::Hidden);
			IsInvincivle = false;
			return;
		}
		
		PMPHUD->PlayerOverlay->InvinciblePanel->SetVisibility(ESlateVisibility::Visible);
		float ElapsedTime = Duration - RemainingTime;
		const float DurationPercent = ElapsedTime / Duration;
		PMPHUD->PlayerOverlay->Invincible->SetPercent(DurationPercent);
		IsInvincivle = true;
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PMPHUD->PlayerOverlay->InvinciblePanel->Slot);
		if (CanvasSlot)
		{
			if (IsBoost && IsStrengthened)
			{
				CanvasSlot->SetPosition(FVector2D(140.f, 0.f));
			}
			else if (!IsBoost && !IsStrengthened)
			{
				CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
			}
			else
			{
				CanvasSlot->SetPosition(FVector2D(70.f, 0.f));
			}
		}
	}
}

void APMPPlayerController::SetHUDSilence(bool visible)
{
	PMPHUD = IsValid(PMPHUD) ? PMPHUD : Cast<APMPHUD>(GetHUD());
	if (IsValid(PMPHUD) && IsValid(PMPHUD->PlayerOverlay))
	{
		if (visible)
			PMPHUD->PlayerOverlay->SilencePanel->SetVisibility(ESlateVisibility::Visible);
		else
			PMPHUD->PlayerOverlay->SilencePanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APMPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APMPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PMPHUD = Cast<APMPHUD>(GetHUD());
	if (PMPHUD)
	{
		if (PMPHUD->PlayerOverlay == nullptr)
			PMPHUD->AddPlayerOveray();

		Cast<APMPCharacter>(GetPawn())->UpdateHUDHP();
	}
	
}

void APMPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
