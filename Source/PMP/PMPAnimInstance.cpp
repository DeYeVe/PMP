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

void UPMPAnimInstance::AnimNotify_BearAttack() const
{
	OnBearAttack.Broadcast();
}
