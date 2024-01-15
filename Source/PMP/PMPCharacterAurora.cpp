// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterAurora.h"

#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APMPCharacterAurora::APMPCharacterAurora()
{	
	MeshCharacter = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshAurora"));
	MeshCharacter->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshCharacter->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));

	if (SM.Succeeded())
	{
		MeshCharacter->SetSkeletalMesh(SM.Object);
	}

	MeshCharacter->SetupAttachment(RootComponent);
	
	Damage = 35;
	MaxHP = 200;
	CurHP = 200;
}

void APMPCharacterAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPCharacterAurora::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPCharacterAurora::OnAttackMontageEnded);
	AnimInstance->OnAuroraAttack.AddUObject(this, &APMPCharacterAurora::CheckAttack);
}

void APMPCharacterAurora::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APMPCharacterAurora::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APMPCharacterAurora, IsActing);
}

void APMPCharacterAurora::Attack()
{
	Super::Attack();
	
	if(IsActing)
		return;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
	
	IsActing = true;	
}

void APMPCharacterAurora::LocalAttack()
{		
	AnimInstance->PlayAuroraAttackMontage(AttackIndex);
	
	AttackIndex = (AttackIndex + 1) % 3;	
}

void APMPCharacterAurora::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPCharacterAurora::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalAttack();
}

void APMPCharacterAurora::CheckAttack()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 80.f;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (DEBUG_FLAG)
	{		
		FVector Vec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + Vec * 0.5f;
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

void APMPCharacterAurora::Skill_1()
{
	Super::Skill_1();
}

void APMPCharacterAurora::Skill_2()
{
	Super::Skill_2();
}

void APMPCharacterAurora::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage != AnimInstance->AuroraAttackMontage)
		return;
	
	IsActing = false;
}