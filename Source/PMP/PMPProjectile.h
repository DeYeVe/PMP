// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PMPProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	MURIEL_NORMAL,
	MURIEL_SKILL_1,
	MURIEL_SKILL_2,	
};

UCLASS()
class PMP_API APMPProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	UParticleSystemComponent* ProjectileFX;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	UParticleSystem* HitFX;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	TEnumAsByte<EProjectileType> eProjectileType;

	UPROPERTY(EditAnywhere, Category=Projectile)
	int32 Damage;

public:
	UFUNCTION()
	void SetDamage(int32 iDamage) { Damage = iDamage; };

	UFUNCTION()
	int32 GetDamage() const { return Damage; };
	
public:	
	APMPProjectile();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:
	UPROPERTY(VisibleAnywhere)
	ACharacter* ProjectileOwner;

public:
	UFUNCTION()
	void SetProjectileOwner(ACharacter* TargetProjectileOwner);
};
