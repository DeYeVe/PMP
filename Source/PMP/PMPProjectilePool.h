// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PMPProjectilePool.generated.h"

UCLASS()
class PMP_API APMPProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APMPProjectilePool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class APMPProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	int32 PoolSize;
	
	TArray<class APMPProjectile*> InactiveProjectiles;
	APMPProjectile* GetPooledProjectile(FTransform SpawnTransform);

	
	void InitProjectileToPool();

	void ReturnProjectileToPool(APMPProjectile* Projectile);
};
