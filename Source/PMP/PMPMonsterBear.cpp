// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonsterBear.h"

#include "PMPAIController.h"
#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

APMPMonsterBear::APMPMonsterBear()
{
	GetCapsuleComponent()->SetCapsuleSize(100.f, 100.f);
	
	MeshMonster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
	MeshMonster->SetupAttachment(GetCapsuleComponent());
	MeshMonster->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshMonster->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	MeshMonster->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear.Enemy_Bear'"));
	
	if (SM.Succeeded())
	{
		MeshMonster->SetSkeletalMesh(SM.Object);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	
	AIControllerClass = APMPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Damage = 12;
	MaxHP = 120;
	CurHP = 120;
}

void APMPMonsterBear::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPMonsterBear::OnAttackMontageEnded);
	AnimInstance->OnBearAttack.AddUObject(this, &APMPMonsterBear::CheckAttack);
}

void APMPMonsterBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMPMonsterBear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPMonsterBear::OnRep_HP(int32 LastHP)
{
	Super::OnRep_HP(LastHP);

	if (CurHP <= 0)
	{
		Die();
		return;
	}
	if (CurHP < LastHP)
	{
		Hit();
	}
}

void APMPMonsterBear::Hit()
{
	Super::Hit();

	AnimInstance->PlayBearHitMontage();
}

void APMPMonsterBear::Attack()
{
	Super::Attack();

	if(IsActing)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("BearAttack"));

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
	
	IsActing = true;
}

void APMPMonsterBear::LocalAttack()
{	
	AnimInstance->PlayBearAttackMontage();
}

void APMPMonsterBear::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPMonsterBear::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
	return;
	
	LocalAttack();
}

void APMPMonsterBear::CheckAttack()
{
	TArray<FHitResult> HitResults;
 	FCollisionQueryParams Params(NAME_None, false, this);
 
 	float AttackRange = 100.f;
 	float AttackRadius = 80.f;
	FVector Offset = GetActorForwardVector() * 50.f;
 
 	bool bResult = GetWorld()->SweepMultiByChannel(
 		OUT HitResults,
 		GetActorLocation() + Offset,
 		GetActorLocation() + GetActorForwardVector() * AttackRange + Offset,
 		FQuat::Identity,
 		ECollisionChannel::ECC_GameTraceChannel1,
 		FCollisionShape::MakeSphere(AttackRadius),
 		Params);
 
 	if (DEBUG_FLAG)
 	{		
 		FVector Vec = GetActorForwardVector() * AttackRange;
 		FVector Center = GetActorLocation() + Vec * 0.5f + Offset;
 		float HalfHeight = AttackRange * 0.5f + AttackRadius;
 		FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
 		FColor DrawColor;
 		if(bResult)
 			DrawColor = FColor::Green;
 		else
 			DrawColor = FColor::Red;
 	
 		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);
 	}
 
 	for (auto HitResult : HitResults)
 	{
 		if (bResult && IsValid(HitResult.GetActor()))
 		{
 			FDamageEvent DamageEvent;
 			HitResult.GetActor()->TakeDamage(GetDamage(), DamageEvent, GetController(), this);
 		}
 	}
}

void APMPMonsterBear::Die()
{
	Super::Die();
}

void APMPMonsterBear::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnAttackMontageEnded(Montage, bInterrupted);

	if (Montage != AnimInstance->BearAttackMontage)
		return;
	
	IsActing = false;
	OnAttackEnd.Broadcast();
}
