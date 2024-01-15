// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPHUD.h"

#include "PMPUserWidget.h"

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
	}

}

void APMPHUD::BeginPlay()
{
	Super::BeginPlay();
}
