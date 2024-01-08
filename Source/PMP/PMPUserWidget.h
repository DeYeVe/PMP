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
	/*
	UPROPERTY(meta=(BindWidget))
	FVector2D EnemyPos;*/
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText;
	
	/*
	UFUNCTION()
	void SetCurAmmo(FString str) { CurAmmo->SetText(FText::FromString(str)); };*/

};
