// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterMuriel.h"

#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "PMPMonster.h"
#include "PMPProjectile.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

APMPCharacterMuriel::APMPCharacterMuriel()
{
	MeshCharacter = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshMuriel"));
	MeshCharacter->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshCharacter->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMuriel/Characters/Heroes/Muriel/Meshes/Muriel_GDC.Muriel_GDC'"));

	if (SM.Succeeded())
	{
		MeshCharacter->SetSkeletalMesh(SM.Object);
	}

	MeshCharacter->SetupAttachment(RootComponent);
	
	DefaultDamage = 20;
	Damage = DefaultDamage;
	MaxHP = 200;
	CurHP = 200;
	
	SkillCooldown[0] = 3.0f;
	SkillCooldown[1] = 5.0f;
	SkillCooldown[2] = 10.0f;

	// projectile
	auto InitProjectile = [this](TSubclassOf<class APMPProjectile>& ProjectileClass, const FString& ProjectilePath)
	{
		const ConstructorHelpers::FClassFinder<AActor> ProjectileFinder(*ProjectilePath);
		if (ProjectileFinder.Class != nullptr)
		{
			ProjectileClass = ProjectileFinder.Class;
		}
	};
	InitProjectile(AttackProjectileClass, TEXT("Blueprint'/Game/Player/Muriel/BP_MurielProjectileNormal.BP_MurielProjectileNormal_C'"));
	InitProjectile(Skill1ProjectileClass, TEXT("Blueprint'/Game/Player/Muriel/BP_MurielProjectileSkill1.BP_MurielProjectileSkill1_C'"));
	InitProjectile(Skill2ProjectileClass, TEXT("Blueprint'/Game/Player/Muriel/BP_MurielProjectileSkill2.BP_MurielProjectileSkill2_C'"));

	const ConstructorHelpers::FObjectFinder<UParticleSystem> FXFinder(TEXT("ParticleSystem'/Game/ParagonMuriel/FX/Particles/Abilities/ConsGround/FX/P_ConsGround_Bubble.P_ConsGround_Bubble'"));
	if (FXFinder.Succeeded())
	{
		Skill_3FX = FXFinder.Object;
	}
}

void APMPCharacterMuriel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPCharacterMuriel::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPCharacterMuriel::OnAttackMontageEnded);
	AnimInstance->OnMurielAttack.AddUObject(this, &APMPCharacterMuriel::SpawnAttack);
	AnimInstance->OnMurielSkill_1.AddUObject(this, &APMPCharacterMuriel::SpawnSkill_1);
	AnimInstance->OnMurielSkill_2.AddUObject(this, &APMPCharacterMuriel::SpawnSkill_2);
	AnimInstance->OnMurielSkill_3.AddUObject(this, &APMPCharacterMuriel::SpawnSkill_3);
}

void APMPCharacterMuriel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	
}

void APMPCharacterMuriel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APMPCharacterMuriel::Attack()
{
	Super::Attack();
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
}

void APMPCharacterMuriel::LocalAttack()
{
	AnimInstance->PlayMurielAttackMontage();

	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
}

void APMPCharacterMuriel::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPCharacterMuriel::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalAttack();
}

void APMPCharacterMuriel::SpawnAttack()
{
	if (IsLocallyControlled() && AttackProjectileClass != nullptr)
	{
		FVector RelativePosition(0.0f, 60.0f, 70.0f);
		FVector RelativeVector = GetActorForwardVector() * RelativePosition.X + GetActorRightVector() * RelativePosition.Y + GetActorUpVector() * RelativePosition.Z;
		FVector StartLocation = GetActorLocation() + RelativeVector;
		FRotator Rotation = GetFollowCamera()->GetComponentRotation();

		FTransform Transform(Rotation, StartLocation);
		
		ServerSpawnProjectile(AttackProjectileClass, Transform, Damage);	
	}
}

void APMPCharacterMuriel::Skill_1()
{
	Super::Skill_1();
	
	if (EnumHasAnyFlags(eStatesFlag, EStateFlags::SILENCED))
		return;
	
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	if (IsSkillOnCooldown[0])
		return;
	
	if (!HasAuthority())
		LocalSkill_1();
	
	ServerSkill_1();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
}

void APMPCharacterMuriel::LocalSkill_1()
{
	AnimInstance->PlayMurielSkill_1Montage();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	
	StartSkillCooldown(0);
}

void APMPCharacterMuriel::ServerSkill_1_Implementation()
{
	MulticastSkill_1();
}

void APMPCharacterMuriel::MulticastSkill_1_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_1();
}

void APMPCharacterMuriel::SpawnSkill_1()
{
	if (IsLocallyControlled() && Skill1ProjectileClass != nullptr)
	{
		FVector RelativePosition(0.0f, 60.0f, 70.0f);
		FVector RelativeVector = GetActorForwardVector() * RelativePosition.X + GetActorRightVector() * RelativePosition.Y + GetActorUpVector() * RelativePosition.Z;
		FVector StartLocation = GetActorLocation() + RelativeVector;
		FRotator Rotation = GetFollowCamera()->GetComponentRotation();

		FTransform Transform(Rotation, StartLocation);

		ServerSpawnProjectile(Skill1ProjectileClass, Transform, float(Damage) * 2.f);	
	}	
}

void APMPCharacterMuriel::Skill_2()
{
	Super::Skill_2();
	
	if (EnumHasAnyFlags(eStatesFlag, EStateFlags::SILENCED))
		return;
	
	if(EnumHasAnyFlags(eStatesFlag, EStateFlags::ACTING))
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	if (IsSkillOnCooldown[1])
		return;
	
	if (!HasAuthority())
		LocalSkill_2();
	
	ServerSkill_2();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
}

void APMPCharacterMuriel::LocalSkill_2()
{
	AnimInstance->PlayMurielSkill_2Montage();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	
	StartSkillCooldown(1);
}

void APMPCharacterMuriel::ServerSkill_2_Implementation()
{
	MulticastSkill_2();
}

void APMPCharacterMuriel::MulticastSkill_2_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_2();
}

void APMPCharacterMuriel::SpawnSkill_2()
{
	if (IsLocallyControlled() && Skill2ProjectileClass != nullptr)
	{
		FVector RelativePosition(0.0f, 60.0f, 70.0f);
		FVector RelativeVector = GetActorForwardVector() * RelativePosition.X + GetActorRightVector() * RelativePosition.Y + GetActorUpVector() * RelativePosition.Z;
		FVector StartLocation = GetActorLocation() + RelativeVector;
		FRotator Rotation = GetFollowCamera()->GetComponentRotation();

		FTransform Transform(Rotation, StartLocation);
		
		ServerSpawnProjectile(Skill2ProjectileClass, Transform, float(Damage) * 1.5f);	
	}
}



void APMPCharacterMuriel::Skill_3()
{
	Super::Skill_3();
	
	if (EnumHasAnyFlags(eStatesFlag, EStateFlags::SILENCED))
		return;
	
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

void APMPCharacterMuriel::LocalSkill_3()
{
	AnimInstance->PlayMurielSkill_3Montage();
	
	EnumAddFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	StartSkillCooldown(2);
}

void APMPCharacterMuriel::ServerSkill_3_Implementation()
{
	MulticastSkill_3();
}

void APMPCharacterMuriel::MulticastSkill_3_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_3();
}

void APMPCharacterMuriel::SpawnSkill_3()
{
	if (Skill_3FX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_3FX, GetActorLocation());
	}
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false);

	float AttackRadius = 500.f;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (DEBUG_FLAG)
	{		
		DrawDebugCylinder(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 1), AttackRadius, 32, bResult ? FColor::Green : FColor::Red, false, 2.f);
	}

	for (auto HitResult : HitResults)
	{
		if (bResult && IsValid(HitResult.GetActor()))
		{
			FDamageEvent DamageEvent;
			if (HitResult.GetActor()->IsA(APMPCharacter::StaticClass()))
			{
				Cast<APMPCharacter>(HitResult.GetActor())->TakeDamage(- float(GetDamage()) * 2.5f, DamageEvent, GetController(), this);
			}
			else
			{
				HitResult.GetActor()->TakeDamage(float(GetDamage()) * 2.5, DamageEvent, GetController(), this);
			}
		}
	}
}

void APMPCharacterMuriel::ServerSpawnProjectile_Implementation(TSubclassOf<APMPProjectile> ProjectileClass,
	const FTransform& SpawnTransform, int32 ProjectileDamage)
{
	MulticastSpawnProjectile(ProjectileClass, SpawnTransform, Damage);	
}

void APMPCharacterMuriel::MulticastSpawnProjectile_Implementation(TSubclassOf<APMPProjectile> ProjectileClass, const FTransform& SpawnTransform, int32 ProjectileDamage)
{
	APMPProjectile* Projectile = GetWorld()->SpawnActor<APMPProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetProjectileOwner(this);
		Projectile->SetDamage(ProjectileDamage);
	}
}

void APMPCharacterMuriel::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EnumRemoveFlags(eStatesFlag, EStateFlags::ACTING);
	GetCharacterMovement()->SetJumpAllowed(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
