// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPMonsterBear.h"
#include "Components/CapsuleComponent.h"

APMPMonsterBear::APMPMonsterBear()
{
	GetCapsuleComponent()->SetCapsuleSize(100.f, 100.f);
	
	MeshMonster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MonsterMesh"));
	MeshMonster->SetupAttachment(GetCapsuleComponent());
	MeshMonster->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshMonster->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	MeshMonster->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear.Enemy_Bear'"));
	
	if (SM.Succeeded())
	{
		MeshMonster->SetSkeletalMesh(SM.Object);
	}
}

void APMPMonsterBear::BeginPlay()
{
	Super::BeginPlay();
}

void APMPMonsterBear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMPMonsterBear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
