// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonsterSpider.h"

#include "PMPAIController.h"
#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

APMPMonsterSpider::APMPMonsterSpider()
{
	GetCapsuleComponent()->SetCapsuleSize(80.f, 80.f);
	
	MeshMonster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
	MeshMonster->SetupAttachment(GetCapsuleComponent());
	MeshMonster->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshMonster->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	MeshMonster->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider'"));

	if (SM.Succeeded())
	{
		MeshMonster->SetSkeletalMesh(SM.Object);
	}
	
	AIControllerClass = APMPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PMPAIController = Cast<APMPAIController>(GetController());
	
	Damage = 10;
	MaxHP = 160;
	CurHP = MaxHP;
}

void APMPMonsterSpider::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPMonsterSpider::OnMontageEnded);
	AnimInstance->OnSpiderAttack.AddUObject(this, &APMPMonsterSpider::CheckAttack);

	AController* MonsterController = GetWorld()->GetFirstPlayerController();
	SetOwner(MonsterController);
}

void APMPMonsterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMPMonsterSpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPMonsterSpider::OnRep_HP(int32 LastHP)
{
	Super::OnRep_HP(LastHP);
}

void APMPMonsterSpider::Hit()
{
	Super::Hit();

	CanAct = false;

	if(IsActing)
		return;
	
	if (!HasAuthority())
		LocalHit();
	
	ServerHit();
}

void APMPMonsterSpider::Attack()
{
	Super::Attack();

	if(IsActing)
		return;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
}

void APMPMonsterSpider::Die()
{
	Super::Die();

	CanAct = false;

	if(IsActing)
		return;

	if (!HasAuthority())
		LocalDie();
	
	ServerDie();
}

void APMPMonsterSpider::LocalHit()
{
	if(EnumHasAnyFlags(eStatesFlag, EEnemyStateFlags::FROZEN))
		return;
	
	AnimInstance->PlaySpiderHitMontage();
}

void APMPMonsterSpider::ServerHit_Implementation()
{
	MulticastHit();
}

void APMPMonsterSpider::MulticastHit_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalHit();
}

void APMPMonsterSpider::LocalAttack()
{
	AnimInstance->PlaySpiderAttackMontage();
}

void APMPMonsterSpider::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPMonsterSpider::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalAttack();
}

void APMPMonsterSpider::CheckAttack()
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

void APMPMonsterSpider::LocalDie()
{
	IsActing = true;
	AnimInstance->IsActing = true;
	AnimInstance->PlaySpiderDieMontage();
	
	if(PMPAIController)
		PMPAIController->Destroy();
}

void APMPMonsterSpider::ServerDie_Implementation()
{
	MulticastDie();
}

void APMPMonsterSpider::MulticastDie_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalDie();
}

void APMPMonsterSpider::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);

	if (Montage == AnimInstance->SpiderAttackMontage)
	{
		OnAttackEnd.Broadcast();
		return;
	}
	else if (Montage == AnimInstance->SpiderHitMontage)
	{
		OnHitEnd.Broadcast();
		CanAct = true;
		return;		
	}
	else if (Montage == AnimInstance->SpiderDieMontage)
	{
		Destroy();
		return;
	}

	return;
}
