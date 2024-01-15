// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PMPHUD.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(VisibleAnywhere)
	class UPMPUserWidget* PlayerOverlay;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> PlayerOverlayClass;
	void AddPlayerOveray();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	class APlayerController* OwningPlayer;
	
};
