// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAnimInstance.h"

#include "PMPCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UPMPAnimInstance::UPMPAnimInstance()
{
	auto InitMontage = [this](const FString& MontagePath, UAnimMontage*& Montage)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(*MontagePath);
		if (MontageFinder.Succeeded())
		{
			Montage = MontageFinder.Object;
		}
	};
	
	InitMontage(TEXT("AnimMontage'/Game/Player/Aurora/Animation/AM_Attack.AM_Attack'"), AuroraAttackMontage);
}

void UPMPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<APMPCharacter>(Pawn);
		if (Character)
		{
			MovementVector =  Character->GetMovementVector();
			IsFalling = Character->GetMovementComponent()->IsFalling();
		}		
	}
}

void UPMPAnimInstance::PlayAuroraAttackMontage(int32 Index)
{
	Montage_Play(AuroraAttackMontage, 1.f);
	Montage_JumpToSection(FName(*FString::FromInt(Index)));
}

void UPMPAnimInstance::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("delegate anim notify attack"));
}
