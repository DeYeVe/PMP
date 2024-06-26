// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonster.h"

#include "PMPAIController.h"
#include "PMPAnimInstance.h"
#include "Chaos/PBDRigidClustering.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APMPMonster::APMPMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 50.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
}

// Called when the game starts or when spawned
void APMPMonster::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);
	
	AnimInstance = Cast<UPMPAnimInstance>(GetMesh()->GetAnimInstance());
	OriginalMaterial = GetMesh()->GetMaterial(0);
}

// Called every frame
void APMPMonster::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APMPMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APMPMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APMPMonster, CurHP);
	DOREPLIFETIME(APMPMonster, IsActing);
}

void APMPMonster::OnRep_HP(int32 LastHP)
{
	if (CurHP <= 0)
	{
		OnSlowReleased();
		OnFrozenReleased();
		Die();
	}
	else
	{
		Hit();
	}
	OnTakeDamageExecuted(LastHP - CurHP);
}

void APMPMonster::Hit()
{
}

void APMPMonster::Attack()
{
}

void APMPMonster::Die()
{
}

float APMPMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                              AActor* DamageCauser)
{
	if (HasAuthority())
		ServerTakeDamage(DamageAmount);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void APMPMonster::ServerTakeDamage_Implementation(float DamageAmount)
{
	CurHP -= DamageAmount;
	
	if (CurHP <= 0)
	{
		OnSlowReleased();
		OnFrozenReleased();
		Die();
	}
	else
	{
		Hit();
	}
	OnTakeDamageExecuted(DamageAmount);
}

void APMPMonster::SetFrozen(float DamageAmount)
{
	if (HasAuthority())
	{
		CurHP -= DamageAmount;
	
		OnTakeDamageExecuted(DamageAmount);
	
		if (CurHP <= 0)
		{
			OnFrozenReleased();
			Die();
		
			return;
		}		
	}
	
	CanAct = false;
	EnumAddFlags(eStatesFlag, EEnemyStateFlags::FROZEN);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
	DynamicMaterial->SetVectorParameterValue("EmissiveColor", FLinearColor(0.0f, 0.0f, 1.0f));
	DynamicMaterial->SetScalarParameterValue("EmissiveIntensity", 10.0f);
	GetMesh()->SetMaterial(0, DynamicMaterial);
	
	AnimInstance->SetPlayRate(0.f);
	AnimInstance->Montage_SetPlayRate(AnimInstance->GetCurrentActiveMontage(), AnimInstance->GetPlayRate());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APMPMonster::OnFrozenReleased, 3.0f, false);
}

void APMPMonster::OnFrozenReleased()
{
	CanAct = true;
	GetMesh()->SetMaterial(0, OriginalMaterial);
    
	EnumRemoveFlags(eStatesFlag, EEnemyStateFlags::FROZEN);;
	AnimInstance->SetPlayRate(1.f);
	AnimInstance->Montage_SetPlayRate(AnimInstance->GetCurrentActiveMontage(), AnimInstance->GetPlayRate());
}

void APMPMonster::SetSlow(float DamageAmount)
{
	if (HasAuthority())
	{
		CurHP -= DamageAmount;
		
		OnTakeDamageExecuted(DamageAmount);
		
		if (CurHP <= 0)
		{
			OnSlowReleased();
			Die();
			
			return;
		}
	}
	
	EnumAddFlags(eStatesFlag, EEnemyStateFlags::SLOW);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
	DynamicMaterial->SetVectorParameterValue("EmissiveColor", FLinearColor(0.0f, 0.3f, 0.0f));
	DynamicMaterial->SetScalarParameterValue("EmissiveIntensity", 10.0f);
	GetMesh()->SetMaterial(0, DynamicMaterial);
	
	AnimInstance->SetPlayRate(1.f / 3.f);
	AnimInstance->Montage_SetPlayRate(AnimInstance->GetCurrentActiveMontage(), AnimInstance->GetPlayRate());
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed / 3.f;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APMPMonster::OnSlowReleased, 5.0f, false);

}

void APMPMonster::OnSlowReleased()
{
	GetMesh()->SetMaterial(0, OriginalMaterial);
    
	EnumRemoveFlags(eStatesFlag, EEnemyStateFlags::SLOW);;
	AnimInstance->SetPlayRate(1.f);
	AnimInstance->Montage_SetPlayRate(AnimInstance->GetCurrentActiveMontage(), AnimInstance->GetPlayRate());

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void APMPMonster::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
