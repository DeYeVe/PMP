// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPProjectile.h"

#include "PMPCharacter.h"
#include "PMPMonster.h"
#include "PMPProjectilePool.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APMPProjectile::APMPProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &APMPProjectile::OnHit);	
	CollisionComp->IgnoreActorWhenMoving(this, true);
	
	RootComponent = CollisionComp;
	ProjectileFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileFX"));
	ProjectileFX->SetupAttachment(CollisionComp);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1200.f;
	ProjectileMovement->MaxSpeed = 1200.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	InitialLifeSpan = 8.0f;
}

void APMPProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{	
	if (eProjectileType == EProjectileType::MURIEL_NORMAL)
	{
		if ((OtherActor != nullptr) && (OtherComp != nullptr))
		{
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(GetDamage(), DamageEvent, ProjectileOwner->GetController(), ProjectileOwner);
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
			Destroy();
		}		
	}
	if (eProjectileType == EProjectileType::MURIEL_SKILL_1)
	{
		if ((OtherActor != nullptr) && (OtherComp != nullptr))
		{
			if (OtherActor->IsA(APMPCharacter::StaticClass()))
			{
				FDamageEvent DamageEvent;
				Cast<APMPCharacter>(OtherActor)->TakeDamage(-GetDamage(), DamageEvent, ProjectileOwner->GetController(), ProjectileOwner);
			}
			else
			{
				FDamageEvent DamageEvent;
				OtherActor->TakeDamage(GetDamage(), DamageEvent, ProjectileOwner->GetController(), ProjectileOwner);
			} 
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
			Destroy();
		}		
	}
	if (eProjectileType == EProjectileType::MURIEL_SKILL_2)
	{
		if ((OtherActor != nullptr) && (OtherComp != nullptr))
		{
			if (OtherActor->IsA(APMPMonster::StaticClass()))
			{
				FDamageEvent DamageEvent;
				Cast<APMPMonster>(OtherActor)->SetSlow(GetDamage());
			}
			else
			{
				FDamageEvent DamageEvent;
				OtherActor->TakeDamage(GetDamage(), DamageEvent, ProjectileOwner->GetController(), ProjectileOwner);
			}
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
			Destroy();
		}		
	}
	if (eProjectileType == EProjectileType::SEVAROG_SKILL_1)
	{
		if ((OtherActor != nullptr) && (OtherComp != nullptr))
		{
			if (OtherActor->IsA(APMPCharacter::StaticClass()))
			{
				FDamageEvent DamageEvent;
				Cast<APMPCharacter>(OtherActor)->TakeDamage(GetDamage(), DamageEvent, ProjectileOwner->GetController(), ProjectileOwner);
			}
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
			if (ProjectilePool)
			{
				ProjectilePool->ReturnProjectileToPool(this);
			}
			else
			{
				Destroy();
			}
		}		
	}
}

void APMPProjectile::SetProjectileOwner(ACharacter* TargetProjectileOwner)
{
	ProjectileOwner = TargetProjectileOwner;
}

