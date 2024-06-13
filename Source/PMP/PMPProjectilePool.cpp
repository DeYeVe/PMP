// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPProjectilePool.h"

#include "PMPBoss.h"
#include "PMPProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APMPProjectilePool::APMPProjectilePool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PoolSize = 40;
	
	// projectile
	const ConstructorHelpers::FClassFinder<AActor> ProjectileFinder(TEXT("Blueprint'/Game/Enemies/Sevarog/BP_SevarogProjectile1.BP_SevarogProjectile1_C'"));
	if (ProjectileFinder.Class != nullptr)
	{
		ProjectileClass = ProjectileFinder.Class;
	}
}

// Called when the game starts or when spawned
void APMPProjectilePool::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APMPProjectilePool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMPProjectilePool::InitProjectileToPool()
{
	APMPBoss* Boss = Cast<APMPBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), APMPBoss::StaticClass()));
	
	for (int32 i = 0; i < PoolSize; ++i)
	{
		APMPProjectile* NewProjectile = GetWorld()->SpawnActor<APMPProjectile>(ProjectileClass, FTransform::Identity);
		if (NewProjectile)
		{
			NewProjectile->SetProjectileOwner(Boss);
			NewProjectile->SetDamage(Boss->GetDamage());
			
			NewProjectile->SetActorEnableCollision(false);
			NewProjectile->SetActorHiddenInGame(true);
			NewProjectile->SetActorTickEnabled(false);

			NewProjectile->GetProjectileMovement()->StopMovementImmediately();
			NewProjectile->GetProjectileMovement()->Velocity = FVector::ZeroVector;
			
			NewProjectile->SetProjectilePool(this);

			InactiveProjectiles.Push(NewProjectile);
		}
	}
}

APMPProjectile* APMPProjectilePool::GetPooledProjectile(FTransform SpawnTransform)
{
	if (InactiveProjectiles.Num() > 0)
	{
		APMPProjectile* Projectile = InactiveProjectiles.Pop();

		Projectile->GetProjectileMovement()->SetActive(true);
		
		Projectile->SetActorTransform(SpawnTransform);
		Projectile->SetActorEnableCollision(true);
		Projectile->SetActorHiddenInGame(false);
		Projectile->SetActorTickEnabled(true);

		Projectile->GetProjectileMovement()->Velocity = SpawnTransform.GetRotation().GetForwardVector() * Projectile->GetProjectileMovement()->InitialSpeed;

		UE_LOG(LogTemp, Warning, TEXT("velo %f %f %f %f"), Projectile->GetProjectileMovement()->Velocity.Size(), SpawnTransform.GetRotation().GetForwardVector().XYZ);
		return Projectile;
	}
	return nullptr;
}

void APMPProjectilePool::ReturnProjectileToPool(APMPProjectile* Projectile)
{
	if (Projectile)
	{
		Projectile->SetActorEnableCollision(false);
		Projectile->SetActorHiddenInGame(true);
		Projectile->SetActorTickEnabled(false);

		Projectile->GetProjectileMovement()->Velocity = FVector::ZeroVector;
		Projectile->GetProjectileMovement()->SetActive(false);		
	
		InactiveProjectiles.Push(Projectile);
	}
}

