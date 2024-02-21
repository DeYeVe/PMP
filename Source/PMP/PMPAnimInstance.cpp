// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAnimInstance.h"

#include "PMPCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UPMPAnimInstance::UPMPAnimInstance()
{
	auto InitMontage = [this](UAnimMontage*& Montage, const FString& MontagePath)
	{
		const ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(*MontagePath);
		if (MontageFinder.Succeeded())
		{
			Montage = MontageFinder.Object;
		}
	};
	
	InitMontage(AuroraAttackMontage, TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Attack.AM_Attack'"));
	InitMontage(AuroraSkill_1Montage, TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill1.AM_Skill1'"));
	InitMontage(AuroraSkill_2Montage, TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill2.AM_Skill2'"));
	InitMontage(AuroraSkill_3Montage, TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Skill3.AM_Skill3'"));
	
	InitMontage(MurielAttackMontage, TEXT("AnimMontage'/Game/Player/Muriel/Animation/AM_Attack.AM_Attack'"));
	InitMontage(MurielSkill_1Montage, TEXT("AnimMontage'/Game/Player/Muriel/Animation/AM_Skill1.AM_Skill1'"));
	InitMontage(MurielSkill_2Montage, TEXT("AnimMontage'/Game/Player/Muriel/Animation/AM_Skill2.AM_Skill2'"));
	InitMontage(MurielSkill_3Montage, TEXT("AnimMontage'/Game/Player/Muriel/Animation/AM_Skill3.AM_Skill3'"));
	
	InitMontage(BearAttackMontage, TEXT("AnimMontage'/Game/Enemies/Bear/AM_Attack.AM_Attack'"));
	InitMontage(BearHitMontage, TEXT("AnimMontage'/Game/Enemies/Bear/AM_Hit.AM_Hit'"));
	InitMontage(BearDieMontage, TEXT("AnimMontage'/Game/Enemies/Bear/AM_Die.AM_Die'"));
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
	
	Montage_Play(AuroraAttackMontage, PlayRate);
	Montage_JumpToSection(FName(*FString::FromInt(Index)));
}

void UPMPAnimInstance::PlayAuroraSkill_1Montage()
{
	if (!IsValid(AuroraSkill_1Montage))
		return;
	
	Montage_Play(AuroraSkill_1Montage, PlayRate);
}

void UPMPAnimInstance::PlayAuroraSkill_2Montage()
{
	if (!IsValid(AuroraSkill_2Montage))
		return;
	
	Montage_Play(AuroraSkill_2Montage, PlayRate);
}

void UPMPAnimInstance::PlayAuroraSkill_3Montage()
{
	if (!IsValid(AuroraSkill_3Montage))
		return;
	
	Montage_Play(AuroraSkill_3Montage, PlayRate);
}

void UPMPAnimInstance::PlayMurielAttackMontage()
{
	if (!IsValid(MurielAttackMontage))
		return;
	
	Montage_Play(MurielAttackMontage, PlayRate);
}

void UPMPAnimInstance::PlayMurielSkill_1Montage()
{
	if (!IsValid(MurielSkill_1Montage))
		return;
	
	Montage_Play(MurielSkill_1Montage, PlayRate);
}

void UPMPAnimInstance::PlayMurielSkill_2Montage()
{
	if (!IsValid(MurielSkill_2Montage))
		return;
	
	Montage_Play(MurielSkill_2Montage, PlayRate);
}

void UPMPAnimInstance::PlayMurielSkill_3Montage()
{
	if (!IsValid(MurielSkill_3Montage))
		return;
	
	Montage_Play(MurielSkill_3Montage, PlayRate);
}

void UPMPAnimInstance::PlayBearAttackMontage()
{
	if (!IsValid(BearAttackMontage))
		return;
	
	Montage_Play(BearAttackMontage, PlayRate);
}

void UPMPAnimInstance::PlayBearHitMontage()
{
	if (!IsValid(BearHitMontage))
		return;
	
	Montage_Play(BearHitMontage, PlayRate);
	
	UE_LOG(LogTemp, Warning, TEXT("hit bear montage playRate %f"), PlayRate);
}

void UPMPAnimInstance::PlayBearDieMontage()
{
	if (!IsValid(BearDieMontage))
		return;
	
	Montage_Play(BearDieMontage, PlayRate);
}

void UPMPAnimInstance::AnimNotify_AuroraAttack() const
{
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

void UPMPAnimInstance::AnimNotify_MurielAttack() const
{
	OnMurielAttack.Broadcast();
}

void UPMPAnimInstance::AnimNotify_MurielSkill_1() const
{
	OnMurielSkill_1.Broadcast();
}

void UPMPAnimInstance::AnimNotify_MurielSkill_2() const
{
	OnMurielSkill_2.Broadcast();
}

void UPMPAnimInstance::AnimNotify_MurielSkill_3() const
{
	OnMurielSkill_3.Broadcast();
}

void UPMPAnimInstance::AnimNotify_BearAttack() const
{
	OnBearAttack.Broadcast();
}
