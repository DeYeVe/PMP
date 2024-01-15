// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonster.h"

#include "PMPAnimInstance.h"
#include "Chaos/PBDRigidClustering.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APMPMonster::APMPMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 50.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APMPMonster::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UPMPAnimInstance>(GetMesh()->GetAnimInstance());
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
	DOREPLIFETIME(APMPMonster, TakenDamage);
}

void APMPMonster::OnRep_HP(int32 LastHP)
{
	OnTakeDamageExecuted();
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
	if (!HasAuthority())
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);;
	
	TakenDamage = DamageAmount;
	CurHP -= DamageAmount;
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APMPMonster::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}