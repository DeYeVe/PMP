// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPCharacterAurora.h"

#include "PMPAnimInstance.h"
#include "Net/UnrealNetwork.h"

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

	AnimInstance->OnMontageEnded.AddDynamic(this, &APMPCharacterAurora::OnAttackMontageEnded);
}

void APMPCharacterAurora::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APMPCharacterAurora::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APMPCharacterAurora, IsActing);
}

void APMPCharacterAurora::Attack()
{
	Super::Attack();

	if (!HasAuthority())
		LocalAttack();
	
	ServerAttack();
}

void APMPCharacterAurora::LocalAttack()
{	
	if(IsActing)
		return;
	
	AnimInstance->PlayAuroraAttackMontage(AttackIndex);
	
	AttackIndex = (AttackIndex + 1) % 3;
	
	IsActing = true;
}

void APMPCharacterAurora::ServerAttack_Implementation()
{
	MulticastAttack();
}

void APMPCharacterAurora::MulticastAttack_Implementation()
{
	if (IsLocallyControlled() && !HasAuthority())
		return;
	
	LocalAttack();
}

void APMPCharacterAurora::Skill_1()
{
	Super::Skill_1();
}

void APMPCharacterAurora::Skill_2()
{
	Super::Skill_2();
}

void APMPCharacterAurora::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsActing = false;
}
