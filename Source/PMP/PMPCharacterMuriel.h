// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPCharacter.h"
#include "PMPCharacterMuriel.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPCharacterMuriel : public APMPCharacter
{
	GENERATED_BODY()

public:
	APMPCharacterMuriel();
	
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
	void SpawnAttack();
	
	virtual void Skill_1() override;
	UFUNCTION()
	void LocalSkill_1();
	UFUNCTION(Server, Reliable)
	void ServerSkill_1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_1();
	UFUNCTION()
	void SpawnSkill_1();
	
	virtual void Skill_2() override;	
	UFUNCTION()
	void LocalSkill_2();
	UFUNCTION(Server, Reliable)
	void ServerSkill_2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_2();
	UFUNCTION()
	void SpawnSkill_2();
	
	virtual void Skill_3() override;
	UFUNCTION()
	void LocalSkill_3();
	UFUNCTION(Server, Reliable)
	void ServerSkill_3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_3();
	UFUNCTION()
	void SpawnSkill_3();	

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APMPProjectile> AttackProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APMPProjectile> Skill1ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APMPProjectile> Skill2ProjectileClass;
	
	UPROPERTY()
	UParticleSystem* SKill_3FX;	
};
