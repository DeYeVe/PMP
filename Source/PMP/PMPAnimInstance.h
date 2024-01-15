// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PMPAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttack);
/**
 * 
 */
UCLASS()
class PMP_API UPMPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPMPAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAuroraAttackMontage(int32 Index);
	
	void PlayBearAttackMontage();
	void PlayBearHitMontage();
	void PlayBearDieMontage();

private:
	UFUNCTION()
	void AnimNotify_AuroraAttack() const;
	
	UFUNCTION()
	void AnimNotify_BearAttack() const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Aurora, Meta=(AllowPrivateAccess=true)) \
	UAnimMontage* AuroraAttackMontage;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=EnemyBear, Meta=(AllowPrivateAccess=true)) \
	UAnimMontage* BearAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=EnemyBear, Meta=(AllowPrivateAccess=true)) \
	UAnimMontage* BearDieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=EnemyBear, Meta=(AllowPrivateAccess=true)) \
	UAnimMontage* BearHitMontage;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector2D MovementVector;

public:
	FOnAttack OnAuroraAttack;
	FOnAttack OnBearAttack;
};
