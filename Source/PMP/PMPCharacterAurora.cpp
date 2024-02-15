// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterAurora.h"

#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "PMPMonster.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

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
	
	// FX
	auto InitFX = [this](const FString& FXPath, UParticleSystem*& FX)
	{
		const ConstructorHelpers::FObjectFinder<UParticleSystem> FXFinder(*FXPath);
		if (FXFinder.Succeeded())
		{
			FX = FXFinder.Object;
		}
	};
	InitFX(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Leap/FX/P_Aurora_Decoy_Frost.P_Aurora_Decoy_Frost'"), SKill_1FX);
	InitFX(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Dash/FX/P_Aurora_Dash_Flare.P_Aurora_Dash_Flare'"), SKill_2FX);
	InitFX(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Ultimate_Warmup.P_Aurora_Ultimate_Warmup'"), SKill_3FX_1);
	InitFX(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Ultimate_InitialBlast.P_Aurora_Ultimate_InitialBlast'"), SKill_3FX_2);
	InitFX(TEXT("ParticleSystem'/Game/ParagonAurora/FX/Particles/Abilities/Ultimate/FX/P_Aurora_Frozen_Ground_Water_Ultimate.P_Aurora_Frozen_Ground_Water_Ultimate'"), SKill_3FX_3);

	
	Damage = 35;
	MaxHP = 200;
	CurHP = 200;
	SkillCooldown[0] = 3.0f;
	SkillCooldown[1] = 5.0f;
	SkillCooldown[2] = 10.0f;
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
	AnimInstance->OnAuroraSkill_1.AddUObject(this, &APMPCharacterAurora::CheckSkill_1);
	AnimInstance->OnAuroraSkill_2.AddUObject(this, &APMPCharacterAurora::CheckSkill_2);
	AnimInstance->OnAuroraSkill_3.AddUObject(this, &APMPCharacterAurora::CheckSkill_3);
}

void APMPCharacterAurora::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AnimInstance->Montage_IsPlaying(AnimInstance->AuroraSkill_2Montage))
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaSeconds * 1000.f, true);
	}
	if (AnimInstance->Montage_IsPlaying(AnimInstance->AuroraSkill_3Montage))
	{
		Skill_3Offset += DeltaSeconds;
		if (Skill_3Offset >= 0.13f && Skill_3Offset <= 0.6f)
			SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 400.f) * DeltaSeconds, true);
	}

}

void APMPCharacterAurora::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APMPCharacterAurora::Attack()
{
	Super::Attack();
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
}

void APMPCharacterAurora::LocalAttack()
{		
	AnimInstance->PlayAuroraAttackMontage(AttackIndex);
	
	AttackIndex = (AttackIndex + 1) % 3;	
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
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

void APMPCharacterAurora::Skill_1()
{
	Super::Skill_1();
	
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	if (IsSkillOnCooldown[0])
		return;

	if (!HasAuthority())
		LocalSkill_1();
	
	ServerSkill_1();
}

void APMPCharacterAurora::LocalSkill_1()
{
	AnimInstance->PlayAuroraSkill_1Montage();
	
	AttackIndex = 0;
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	StartSkillCooldown(0);
}

void APMPCharacterAurora::ServerSkill_1_Implementation()
{
	MulticastSkill_1();
}

void APMPCharacterAurora::MulticastSkill_1_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_1();
}

void APMPCharacterAurora::CheckSkill_1()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 180.f;
	float AttackWidth = 360.f;
	float AttackHeight = 100.f;
	float HalfExtent = AttackRange / 2.0f;
	FVector BoxExtent = FVector(HalfExtent, AttackWidth, AttackHeight);
	FTransform BoxTransform = FTransform(GetActorForwardVector().Rotation(), GetActorLocation() + GetActorForwardVector() * HalfExtent);

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		BoxTransform.GetLocation(),
		BoxTransform.GetLocation(),
		BoxTransform.GetRotation(),
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeBox(BoxExtent),
		Params);

	if (DEBUG_FLAG)
	{
		DrawDebugBox(GetWorld(), BoxTransform.GetLocation(), BoxExtent, FQuat(BoxTransform.GetRotation()), bResult ? FColor::Green : FColor::Red, false, 2.f);
	}

	for (auto HitResult : HitResults)
	{
		if (bResult && IsValid(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(GetDamage() * 2, DamageEvent, GetController(), this);
		}
	}
		
	if (SKill_2FX != nullptr)
	{
		auto SpawnFX = [this](float f1, float f2, float f3)
		{
			FRotator SpawnRotation = GetActorRotation();
			FVector SpawnLocation = GetActorLocation() + FQuat(SpawnRotation) * FVector(f1, f2, f3);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SKill_2FX, SpawnLocation, SpawnRotation + FRotator(0.f, 180.f, 0.f));
		};
		SpawnFX( 270.f, -360.f, -50.f );
		SpawnFX( 320.f, -180.f, -50.f );
		SpawnFX( 370.f, 0.f, -50.f );
		SpawnFX( 320.f, 180.f, -50.f );
		SpawnFX( 270.f, 360.f, -50.f );
	}
}

void APMPCharacterAurora::Skill_2()
{
	Super::Skill_2();
	
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	if (IsSkillOnCooldown[1])
		return;

	if (!HasAuthority())
		LocalSkill_2();
	
	ServerSkill_2();
}

void APMPCharacterAurora::LocalSkill_2()
{
	AnimInstance->PlayAuroraSkill_2Montage();
	
	AttackIndex = 0;
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	StartSkillCooldown(1);
	
	if (SKill_2FX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(SKill_2FX, GetMesh(), FName("Skill2"),
			FVector(0.f, -80.f, 100.f ), FRotator(0.f, 90.f, 0.f));
	}
}

void APMPCharacterAurora::ServerSkill_2_Implementation()
{
	MulticastSkill_2();
}

void APMPCharacterAurora::MulticastSkill_2_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_2();
}

void APMPCharacterAurora::CheckSkill_2()
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
	
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, bResult ? FColor::Green : FColor::Red, false, 2.f);
}

	for (auto HitResult : HitResults)
	{
		if (bResult && IsValid(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(float(GetDamage()) * 1.2, DamageEvent, GetController(), this);
		}
	}
}

void APMPCharacterAurora::Skill_3()
{
	Super::Skill_3();
	
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	if (IsSkillOnCooldown[2])
		return;

	if (!HasAuthority())
		LocalSkill_3();
	
	ServerSkill_3();
}

void APMPCharacterAurora::LocalSkill_3()
{
	AnimInstance->PlayAuroraSkill_3Montage();
	
	AttackIndex = 0;	
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCharacterMovement()->GravityScale = 0.f;
	
	StartSkillCooldown(2);
	if (SKill_3FX_1 != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SKill_3FX_1, GetActorLocation());
	}
}

void APMPCharacterAurora::ServerSkill_3_Implementation()
{
	MulticastSkill_3();
}

void APMPCharacterAurora::MulticastSkill_3_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_3();
}

void APMPCharacterAurora::CheckSkill_3()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRadius = 500.f;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (DEBUG_FLAG)
	{		
		FVector Center = GetActorLocation();
	
		DrawDebugCylinder(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 1), 500.0f, 32, bResult ? FColor::Green : FColor::Red, false, 2.f);
	}

	for (auto HitResult : HitResults)
	{
		if (bResult && IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->IsA(APMPMonster::StaticClass()))
			{
				Cast<APMPMonster>(HitResult.GetActor())->SetFrozen(float(GetDamage()) * 1.4);
			}
		}
	}
	
	if (SKill_3FX_2 != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SKill_3FX_2, GetActorLocation() + FVector(0.f, 0.f, -96.f ));
	}
	if (SKill_3FX_3 != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SKill_3FX_3, GetActorLocation());
	}
}

void APMPCharacterAurora::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EnumRemoveFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Skill_3Offset = 0.f;
	GetCharacterMovement()->GravityScale = 1.f;
}
