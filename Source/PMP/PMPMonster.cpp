// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonster.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APMPMonster::APMPMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 50.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void APMPMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APMPMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APMPMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APMPMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurHP -= DamageAmount;
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}