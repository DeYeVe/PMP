// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PMPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetHUDHP(int32 CurHP, int32 MaxHP);

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere)
	class APMPHUD* PMPHUD;
};
