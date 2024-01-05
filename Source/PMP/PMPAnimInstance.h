// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PMPAnimInstance.generated.h"

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

private:
	UFUNCTION()
	void AnimNotify_Attack();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Aurora, Meta=(AllowPrivateAccess=true)) \
	UAnimMontage* AuroraAttackMontage;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector2D MovementVector;
};
