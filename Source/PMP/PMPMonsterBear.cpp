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
	
	
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear.Enemy_Bear'"));
	
	if (SM.Succeeded())
	{
		MeshMonster->SetSkeletalMesh(SM.Object);
	}
	
	AIControllerClass = APMPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PMPAIController = Cast<APMPAIController>(GetController());

	
	Damage = 12;
	MaxHP = 240;
	CurHP = MaxHP;
}

void APMPMonsterBear::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPMonsterBear::OnMontageEnded);
	AnimInstance->OnBearAttack.AddUObject(this, &APMPMonsterBear::CheckAttack);

	AController* MonsterController = GetWorld()->GetFirstPlayerController();
	SetOwner(MonsterController);
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
}

void APMPMonsterBear::Hit()
{
	Super::Hit();

	CanAct = false;

	if(IsActing)
		return;
	
	if (!HasAuthority())
		LocalHit();
	
	ServerHit();
}

void APMPMonsterBear::Attack()
{
	Super::Attack();

	if(IsActing)
		return;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
}

void APMPMonsterBear::Die()
{
	Super::Die();

	CanAct = false;

	if(IsActing)
		return;

	if (!HasAuthority())
		LocalDie();
	
	ServerDie();
}

void APMPMonsterBear::LocalHit()
{
	if(EnumHasAnyFlags(eStatesFlag, EEnemyStateFlags::FROZEN))
		return;
	
	AnimInstance->PlayBearHitMontage();
}

void APMPMonsterBear::ServerHit_Implementation()
{
	MulticastHit();
}

void APMPMonsterBear::MulticastHit_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalHit();
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
 		
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, bResult ? FColor::Green : FColor::Red, false, 2.f);
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

void APMPMonsterBear::LocalDie()
{
	IsActing = true;
	AnimInstance->IsActing = true;
	AnimInstance->PlayBearDieMontage();
	
	if(PMPAIController)
		PMPAIController->Destroy();
}

void APMPMonsterBear::ServerDie_Implementation()
{
	MulticastDie();
}

void APMPMonsterBear::MulticastDie_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalDie();
}

void APMPMonsterBear::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);

	if (Montage == AnimInstance->BearAttackMontage)
	{
		OnAttackEnd.Broadcast();
	}
	else if (Montage == AnimInstance->BearHitMontage)
	{
		OnHitEnd.Broadcast();
		CanAct = true;
	}
	else if (Montage == AnimInstance->BearDieMontage)
	{
		Destroy();
	}
}