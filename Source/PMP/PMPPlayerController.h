// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PMPPlayerController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBuff : uint8
{
	Boost,
	Strengthen,
	Invincible
};

UCLASS()
class PMP_API APMPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetHUDHP(int32 CurHP, int32 MaxHP);
	
	UFUNCTION()
	void SetHUDCooldown1(float CurCooldown, float MaxCooldown);
	UFUNCTION()
	void SetHUDCooldown2(float CurCooldown, float MaxCooldown);
	UFUNCTION()
	void SetHUDCooldown3(float CurCooldown, float MaxCooldown);
	
	UFUNCTION()
	void SetHUDBuffBoost(float RemainingTime, float Duration);
	UFUNCTION()
	void SetHUDBufStrengthen(float RemainingTime, float Duration);
	UFUNCTION()
	void SetHUDBuffInvincible(float RemainingTime, float Duration);
	
	UFUNCTION()
	void SetHUDSilence(bool visible);

	UPROPERTY()
	bool IsBoost;
	UPROPERTY()
	bool IsStrengthened;
	UPROPERTY()
	bool IsInvincivle;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere)
	class APMPHUD* PMPHUD;
};
