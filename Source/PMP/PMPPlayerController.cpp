// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPPlayerController.h"

#include "PMPCharacter.h"
#include "PMPHUD.h"
#include "PMPUserWidget.h"
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
