// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterAurora.h"

APMPCharacterAurora::APMPCharacterAurora()
{	
	MeshCharacter = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshAurora"));
	MeshCharacter->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshCharacter->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));

	if (SM.Succeeded())
	{
		MeshCharacter->SetSkeletalMesh(SM.Object);
	}

	MeshCharacter->SetupAttachment(RootComponent);
}

void APMPCharacterAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APMPCharacterAurora::BeginPlay()
{
	Super::BeginPlay();
}

void APMPCharacterAurora::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
