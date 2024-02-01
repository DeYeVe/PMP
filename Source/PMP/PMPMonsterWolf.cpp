// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonsterWolf.h"

#include "PMPAIController.h"
#include "PMPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APMPMonsterWolf::APMPMonsterWolf()
{
	GetCapsuleComponent()->SetCapsuleSize(80.f, 80.f);
	
	MeshMonster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
	MeshMonster->SetupAttachment(GetCapsuleComponent());
	MeshMonster->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshMonster->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	MeshMonster->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Wolf/SK_Enemy_Wolf_Armored.SK_Enemy_Wolf_Armored'"));

	if (SM.Succeeded())
	{
		MeshMonster->SetSkeletalMesh(SM.Object);
	}
	
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	
	AIControllerClass = APMPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	Damage = 10;
	MaxHP = 100;
	CurHP = 100;
}

void APMPMonsterWolf::BeginPlay()
{
	Super::BeginPlay();
}

void APMPMonsterWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMPMonsterWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPMonsterWolf::OnRep_HP(int32 LastHP)
{
	Super::OnRep_HP(LastHP);
}

void APMPMonsterWolf::Hit()
{
	Super::Hit();
}

void APMPMonsterWolf::Attack()
{
	Super::Attack();
}

void APMPMonsterWolf::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);	
	OnAttackEnd.Broadcast();
}
