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
	UFUNCTION()
	void LocalSkill_1();
	UFUNCTION(Server, Reliable)
	void ServerSkill_1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_1();
	UFUNCTION()
	void CheckSkill_1();
	
	virtual void Skill_2() override;	
	UFUNCTION()
	void LocalSkill_2();
	UFUNCTION(Server, Reliable)
	void ServerSkill_2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_2();
	UFUNCTION()
	void CheckSkill_2();
	
	virtual void Skill_3() override;
	UFUNCTION()
	void LocalSkill_3();
	UFUNCTION(Server, Reliable)
	void ServerSkill_3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_3();
	UFUNCTION()
	void CheckSkill_3();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	int32 AttackIndex = 0;

	UPROPERTY()
	float Skill_3Offset;

private:
	UPROPERTY()
	UParticleSystem* SKill_1FX;
	UPROPERTY()
	UParticleSystem* SKill_2FX;
	UPROPERTY()
	UParticleSystem* SKill_3FX_1;
	UPROPERTY()
	UParticleSystem* SKill_3FX_2;
	UPROPERTY()
	UParticleSystem* SKill_3FX_3;	
};
