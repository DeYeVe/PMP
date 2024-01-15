// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPCharacter.h"
#include "PMPCharacterAurora.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPCharacterAurora : public APMPCharacter
{
	GENERATED_BODY()

public:
	APMPCharacterAurora();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Attack() override;
	UFUNCTION()
	void LocalAttack();
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();
	UFUNCTION()
	void CheckAttack();
	
	virtual void Skill_1() override;
	
	virtual void Skill_2() override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsActing = false;

	UPROPERTY()
	int32 AttackIndex = 0;
	
};
