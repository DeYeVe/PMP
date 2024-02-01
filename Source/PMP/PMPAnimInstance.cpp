// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAnimInstance.h"

#include "PMPCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UPMPAnimInstance::UPMPAnimInstance()
{
	auto InitMontage = [this](const FString& MontagePath, UAnimMontage*& Montage)
	{
		const ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(*MontagePath);
		if (MontageFinder.Succeeded())
		{
			Montage = MontageFinder.Object;
		}
	};
	
	InitMontage(TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Attack.AM_Attack'"), AuroraAttackMontage);
	InitMontage(TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill1.AM_Skill1'"), AuroraSkill_1Montage);
	InitMontage(TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill2.AM_Skill2'"), AuroraSkill_2Montage);
	InitMontage(TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill3.AM_Skill3'"), AuroraSkill_3Montage);
	
	InitMontage(TEXT("AnimMontage'/Game/Enemies/Bear/AM_Attack.AM_Attack'"), BearAttackMontage);
	InitMontage(TEXT("AnimMontage'/Game/Enemies/Bear/AM_Hit.AM_Hit'"), BearHitMontage);
	InitMontage(TEXT("AnimMontage'/Game/Enemies/Bear/AM_Die.AM_Die'"), BearDieMontage);
}

void UPMPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		if (const auto Character = Cast<APMPCharacter>(Pawn))
		{
			MovementVector =  Character->GetMovementVector();
			IsFalling = Character->GetMovementComponent()->IsFalling();
		}		
	}
}

void UPMPAnimInstance::PlayAuroraAttackMontage(int32 Index)
{
	if (!IsValid(AuroraAttackMontage))
		return;
	
	Montage_Play(AuroraAttackMontage, 1.f);
	Montage_JumpToSection(FName(*FString::FromInt(Index)));
}

void UPMPAnimInstance::PlayAuroraSkill_1Montage()
{
	if (!IsValid(AuroraSkill_1Montage))
		return;
	
	Montage_Play(AuroraSkill_1Montage, 1.f);
}

void UPMPAnimInstance::PlayAuroraSkill_2Montage()
{
	if (!IsValid(AuroraSkill_2Montage))
		return;
	
	Montage_Play(AuroraSkill_2Montage, 1.f);
}

void UPMPAnimInstance::PlayAuroraSkill_3Montage()
{
	if (!IsValid(AuroraSkill_3Montage))
		return;
	
	Montage_Play(AuroraSkill_3Montage, 1.f);
}

void UPMPAnimInstance::PlayBearAttackMontage()
{
	if (!IsValid(BearAttackMontage))
		return;
	
	Montage_Play(BearAttackMontage, 1.f);
}

void UPMPAnimInstance::PlayBearHitMontage()
{
	if (!IsValid(BearHitMontage))
		return;
	
	Montage_Play(BearHitMontage, 1.f);
}

void UPMPAnimInstance::PlayBearDieMontage()
{
	if (!IsValid(BearDieMontage))
		return;
	
	Montage_Play(BearDieMontage, 1.f);
}

void UPMPAnimInstance::AnimNotify_AuroraAttack() const
{
	UE_LOG(LogTemp, Warning, TEXT("delegate anim notify attack"));
	OnAuroraAttack.Broadcast();
}

void UPMPAnimInstance::AnimNotify_AuroraSkill_1() const
{
	OnAuroraSkill_1.Broadcast();
}

void UPMPAnimInstance::AnimNotify_AuroraSkill_2() const
{
	OnAuroraSkill_2.Broadcast();
}

void UPMPAnimInstance::AnimNotify_AuroraSkill_3() const
{
	OnAuroraSkill_3.Broadcast();
}

void UPMPAnimInstance::AnimNotify_BearAttack() const
{
	OnBearAttack.Broadcast();
}
