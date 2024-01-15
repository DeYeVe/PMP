// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterFey.h"

APMPCharacterFey::APMPCharacterFey()
{
	MeshCharacter = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFey"));
	MeshCharacter->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshCharacter->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonFey/Characters/Heroes/Fey/Meshes/Fey_GDC.Fey_GDC'"));

	if (SM.Succeeded())
	{
		MeshCharacter->SetSkeletalMesh(SM.Object);
	}

	MeshCharacter->SetupAttachment(RootComponent);
	
	Damage = 25;
	MaxHP = 200;
	CurHP = 200;
}

void APMPCharacterFey::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPCharacterFey::BeginPlay()
{
	Super::BeginPlay();
}

void APMPCharacterFey::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
