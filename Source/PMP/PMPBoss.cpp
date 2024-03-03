// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPBoss.h"

#include "PMPAIControllerBoss.h"
#include "PMPAnimInstance.h"
#include "PMPCharacter.h"
#include "PMPGameMode.h"
#include "PMPProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/DecalActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"

// Sets default values
APMPBoss::APMPBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	GetCapsuleComponent()->InitCapsuleSize(200.f, 300.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	
	MeshBoss = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
	MeshBoss->SetupAttachment(GetCapsuleComponent());
	MeshBoss->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshBoss->SetRelativeLocation(FVector(0.f, 0.f, -300.f));
	MeshBoss->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	MeshBoss->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Skins/Tier_1/Sevarog_Red/Meshes/SevarogBloodred.SevarogBloodred'"));
	
	if (SM.Succeeded())
	{
		MeshBoss->SetSkeletalMesh(SM.Object);
	}

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
	AIControllerClass = APMPAIControllerBoss::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PMPAIController = Cast<APMPAIControllerBoss>(GetController());

	Damage = 30;
	MaxHP = 1500;
	CurHP = MaxHP;
	
	// projectile
	const ConstructorHelpers::FClassFinder<AActor> ProjectileFinder(TEXT("Blueprint'/Game/Enemies/Sevarog/BP_SevarogProjectile1.BP_SevarogProjectile1_C'"));
	if (ProjectileFinder.Class != nullptr)
	{
		Skill1ProjectileClass = ProjectileFinder.Class;
	}

	// fx
	auto InitFX = [this](UParticleSystem*& ParticleSystem, const FString& FXPath)
	{
		const ConstructorHelpers::FObjectFinder<UParticleSystem> FXFinder(*FXPath);
		if (FXFinder.Succeeded())
		{
			ParticleSystem = FXFinder.Object;
		}
	};
	InitFX(Skill_3FX, TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Subjugate/FX/P_SubjugateSwirls.P_SubjugateSwirls'"));
	InitFX(Skill_4FX, TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltActivate.P_UltActivate'"));
	
	// decal
	const ConstructorHelpers::FClassFinder<ADecalActor> DecalFinder(TEXT("Blueprint'/Game/Enemies/Sevarog/BP_SevarogSkill3Decal.BP_SevarogSkill3Decal_C'"));
	if (DecalFinder.Class != nullptr)
	{
		Skill3DecalClass = DecalFinder.Class;
	}
}

// Called when the game starts or when spawned
void APMPBoss::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);	
	
	AnimInstance = Cast<UPMPAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPBoss::OnMontageEnded);
	AnimInstance->OnSevarogAttack.AddUObject(this, &APMPBoss::CheckAttack);
	AnimInstance->OnSevarogSkill_1.AddUObject(this, &APMPBoss::SpawnSkill_1);
	AnimInstance->OnSevarogSkill_2.AddUObject(this, &APMPBoss::SpawnSkill_2);
	AnimInstance->OnSevarogSkill_3.AddUObject(this, &APMPBoss::SpawnSkill_3);
	AnimInstance->OnSevarogSkill_4.AddUObject(this, &APMPBoss::SpawnSkill_4);

	
	AController* BossController = GetWorld()->GetFirstPlayerController();
	SetOwner(BossController);	
}

// Called every frame
void APMPBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Center = GetActorLocation();
	float SearchRadius = 2000.f;
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		if (!OverlapResults.IsEmpty())
		{
			for (auto& OverlapResult : OverlapResults)
			{
				if ((OverlapResult.GetActor()->IsA(APMPCharacter::StaticClass())))
				{
					OnInRange();
				}					
			}
		}
	}
}

// Called to bind functionality to input
void APMPBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APMPBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APMPBoss, CurHP);
	DOREPLIFETIME(APMPBoss, TakenDamage);
	DOREPLIFETIME(APMPBoss, IsActing);
}

void APMPBoss::OnRep_HP(int32 LastHP)
{
	OnTakeDamageExecuted();
}

void APMPBoss::Attack()
{
	CanAct = false;

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
}

void APMPBoss::LocalAttack()
{
	AnimInstance->PlaySevarogAttackMontage();
}

void APMPBoss::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPBoss::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalAttack();
}

void APMPBoss::CheckAttack()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
 
	float AttackRadius = 500.f;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
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
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(float(GetDamage()), DamageEvent, GetController(), this);
		}
	}
}

void APMPBoss::Skill_1()
{
	CanAct = false;
	
	if (!HasAuthority())
		LocalSkill_1();
	
	ServerSkill_1();
}

void APMPBoss::LocalSkill_1()
{
	AnimInstance->PlaySevarogSkill_1Montage();
}

void APMPBoss::ServerSkill_1_Implementation()
{
	MulticastSkill_1();
}

void APMPBoss::MulticastSkill_1_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_1();
}

void APMPBoss::SpawnSkill_1()
{
	if (Skill1ProjectileClass != nullptr)
	{
		if (Target == nullptr)
			SearchTarget();
		
		if (Target != nullptr)
		{
			if (HasAuthority() && Skill1ProjectileClass != nullptr)
			{
				FVector RelativePosition(0.0f, 150.0f, 500.0f);
				FVector RelativeVector = GetActorForwardVector() * RelativePosition.X + GetActorRightVector() * RelativePosition.Y + GetActorUpVector() * RelativePosition.Z;
				FVector StartLocation = GetActorLocation() + RelativeVector;
			
				FVector TargetLocation = Target->GetActorLocation();
				FVector Direction = TargetLocation - StartLocation;
				FVector NormalDirection = Direction;
				NormalDirection.Normalize();			
				FRotator Rotation = NormalDirection.Rotation();
				FTransform Transform(Rotation, StartLocation);

				MulticastSpawnProjectile(Skill1ProjectileClass, Transform, GetDamage());	
			}
		}
	}	
}

void APMPBoss::Skill_2()
{
	CanAct = false;
	
	if (!HasAuthority())
		LocalSkill_2();
	
	ServerSkill_2();
}

void APMPBoss::LocalSkill_2()
{
	AnimInstance->PlaySevarogSkill_2Montage();
}

void APMPBoss::ServerSkill_2_Implementation()
{
	MulticastSkill_2();
}

void APMPBoss::MulticastSkill_2_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_2();
}

void APMPBoss::SpawnSkill_2()
{
	if (HasAuthority() && Skill1ProjectileClass != nullptr)
	{
		Skill2Offset %= 30;
		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -200.0f);

		for (int i = 0; i < 12; ++i)
		{
			FRotator Rotation = FRotator(0.f, Skill2Offset + 30.f * i, 0.f);
			FTransform Transform(Rotation, StartLocation);
		
			MulticastSpawnProjectile(Skill1ProjectileClass, Transform, GetDamage());	
		}
		Skill2Offset += 15;
	}	
}

void APMPBoss::Skill_3()
{
	CanAct = false;
	
	if (!HasAuthority())
		LocalSkill_3();
	
	ServerSkill_3();
}

void APMPBoss::LocalSkill_3()
{
	AnimInstance->PlaySevarogSkill_3Montage();
}

void APMPBoss::ServerSkill_3_Implementation()
{
	MulticastSkill_3();
}

void APMPBoss::MulticastSkill_3_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_3();
}

void APMPBoss::SpawnSkill_3()
{
	if (HasAuthority())
	{
		GenerateRandomLocationAndSync();
	}
}

void APMPBoss::GenerateRandomLocationAndSync()
{
	if (Target == nullptr)
		SearchTarget();
		
	if (Target != nullptr)
	{
		FVector TargetLocation = Target->GetActorLocation();
		
		for (int i = 0; i < 5; ++i)
		{
			FVector RandomLocation = TargetLocation;
			
			if (i != 0)
			{
				RandomLocation = RandomLocation + FVector(FMath::RandPointInCircle(510.f), 0.f);
			}
			MulticastSyncRandomLocation(RandomLocation);
		}
	}
}

void APMPBoss::MulticastSyncRandomLocation_Implementation(const FVector& RandomLocation)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && PlayerController->IsLocalPlayerController())
		{
			SyncRandomLocation(PlayerController, RandomLocation);
		}
	}
}

void APMPBoss::SyncRandomLocation_Implementation(APlayerController* PlayerController, const FVector& RandomLocation)
{
	FVector TargetLocation = RandomLocation;
	TargetLocation.Z = -10.f;
	FTransform Transform(TargetLocation);
		
	ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(Skill3DecalClass, Transform);
	Decal->SetLifeSpan(1.5f);
		
	TargetLocation.Z = 10.f;
	FTimerHandle TimerHandleFX;
	GetWorldTimerManager().SetTimer(TimerHandleFX, [&, TargetLocation]()
	{
		if (Skill_3FX != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_3FX, TargetLocation);
		}		
	}, 1.f, false);
		
	FTimerHandle TimerHandleColl;
	GetWorldTimerManager().SetTimer(TimerHandleColl, [&, TargetLocation]()
	{
		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params(NAME_None, false, this);

		float AttackRadius = 170.f;

		bool bResult = GetWorld()->SweepMultiByChannel(
			OUT HitResults,
			TargetLocation,
			TargetLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(AttackRadius),
			Params);
			
		if (DEBUG_FLAG)
		{		
			DrawDebugCylinder(GetWorld(), TargetLocation, TargetLocation + FVector(0, 0, 1), AttackRadius, 32, bResult ? FColor::Green : FColor::Red, false, 2.f);
		}

		for (auto HitResult : HitResults)
		{
			if (bResult && IsValid(HitResult.GetActor()))
			{
				FDamageEvent DamageEvent;
				if (HitResult.GetActor()->IsA(APMPCharacter::StaticClass()))
				{
					HitResult.GetActor()->TakeDamage(float(GetDamage()) * 1.5f, DamageEvent, GetController(), this);
				}
			}
		}
	}, 1.5f, false);
}

void APMPBoss::Skill_4()
{
	CanAct = false;
	
	if (!HasAuthority())
		LocalSkill_4();
	
	ServerSkill_4();
}

void APMPBoss::LocalSkill_4()
{
	AnimInstance->PlaySevarogSkill_4Montage();
}

void APMPBoss::ServerSkill_4_Implementation()
{
	MulticastSkill_4();
}

void APMPBoss::MulticastSkill_4_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalSkill_4();
}

void APMPBoss::SpawnSkill_4()
{
	if (Target == nullptr)
		SearchTarget();
		
	if (Target != nullptr)
	{
		FVector TargetLocation = Target->GetActorLocation();
		
		if (Skill_4FX != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skill_4FX, TargetLocation);
		}

		Target->SetSilence();
	}
}

void APMPBoss::MulticastSpawnProjectile_Implementation(TSubclassOf<APMPProjectile> ProjectileClass,
	const FTransform& SpawnTransform, int32 ProjectileDamage)
{
	APMPProjectile* Projectile = GetWorld()->SpawnActor<APMPProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetProjectileOwner(this);
		Projectile->SetDamage(ProjectileDamage);
	}
}

float APMPBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                           AActor* DamageCauser)
{
	
	TakenDamage = DamageAmount;
	
	if (!HasAuthority())
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurHP -= DamageAmount;
	
	if (CurHP <= 0)
	{
		//Die();
	}
	
	OnTakeDamageExecuted();
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APMPBoss::SearchTarget()
{
	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float SearchRadius = 1500.f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, this);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		if (!OverlapResults.IsEmpty())
		{
			APMPCharacter* PMPCharacter = nullptr;
			float MinDistance = 0.f;
            		
			for (auto& OverlapResult : OverlapResults)
			{
				if (PMPCharacter == nullptr)
				{
					PMPCharacter = Cast<APMPCharacter>(OverlapResult.GetActor());
					MinDistance = GetDistanceTo(PMPCharacter);
				}
				else if (MinDistance > GetDistanceTo(Cast<APMPCharacter>(OverlapResult.GetActor())))
				{
					PMPCharacter = Cast<APMPCharacter>(OverlapResult.GetActor());
					MinDistance = GetDistanceTo(PMPCharacter);
				}
			}
			if(DEBUG_FLAG)	
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
			
			Target = PMPCharacter;
 		
			return;
		}
		if(DEBUG_FLAG)	
			DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		if(DEBUG_FLAG)	
			DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
}

void APMPBoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CanAct = true;
}

