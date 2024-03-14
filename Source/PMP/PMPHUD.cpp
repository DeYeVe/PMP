// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPHUD.h"

#include "PMPCharacterAurora.h"
#include "PMPCharacterMuriel.h"
#include "PMPUserWidget.h"
#include "Components/CanvasPanel.h"

void APMPHUD::DrawHUD()
{
	Super::DrawHUD();
}

void APMPHUD::AddPlayerOveray()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerOverlayClass)
	{
		PlayerOverlay = CreateWidget<UPMPUserWidget>(PlayerController, PlayerOverlayClass);
		PlayerOverlay->AddToViewport();
		if (GetOwningPawn() == nullptr)
			return;
		
		if (!GetOwningPawn()->IsA(APMPCharacterAurora::StaticClass()))
			PlayerOverlay->AuroraSkill->SetVisibility(ESlateVisibility::Hidden);
		else if (!GetOwningPawn()->IsA(APMPCharacterMuriel::StaticClass()))
			PlayerOverlay->MurielSkill->SetVisibility(ESlateVisibility::Hidden);
	}

}

void APMPHUD::BeginPlay()
{
	Super::BeginPlay();
	PlayerOverlay->BoostPanel->SetVisibility(ESlateVisibility::Hidden);
	PlayerOverlay->StrengthenPanel->SetVisibility(ESlateVisibility::Hidden);
	PlayerOverlay->InvinciblePanel->SetVisibility(ESlateVisibility::Hidden);
	PlayerOverlay->SilencePanel->SetVisibility(ESlateVisibility::Hidden);
			
}
