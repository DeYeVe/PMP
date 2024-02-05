// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterMuriel.h"

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
	
	Damage = 25;
	MaxHP = 200;
	CurHP = 200;
}

void APMPCharacterMuriel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPCharacterMuriel::BeginPlay()
{
	Super::BeginPlay();
}

void APMPCharacterMuriel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
