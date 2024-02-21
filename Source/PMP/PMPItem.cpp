// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPItem.h"

#include "PMPCharacter.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
APMPItem::APMPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBoxComponent"));
	BoxComponent->InitBoxExtent(FVector(25.f, 25.f, 50.f));
	BoxComponent->SetCollisionProfileName(TEXT("Item"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.f));
	SetActorEnableCollision(true);
	
	SetRootComponent(BoxComponent);
	
	MeshItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	MeshItem->SetCollisionProfileName(TEXT("Item"));
	MeshItem->SetupAttachment(RootComponent);

	auto InitNiagaraSystem = [this](UNiagaraSystem*& NiagaraSystem, const FString& SystemPath)
	{
		const ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(*SystemPath);
		if (NiagaraSystemAsset.Succeeded())
		{
			NiagaraSystem = NiagaraSystemAsset.Object;
		}
	};
	
	InitNiagaraSystem(NiagaraBoost, TEXT("NiagaraSystem'/Game/Item/NS_Boost.NS_Boost'"));
	InitNiagaraSystem(NiagaraStrengthen, TEXT("NiagaraSystem'/Game/Item/NS_Strengthen.NS_Strengthen'"));
	InitNiagaraSystem(NiagaraInvincible, TEXT("NiagaraSystem'/Game/Item/NS_Invincible.NS_Invincible'"));

}

// Called when the game starts or when spawned
void APMPItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APMPItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rotation = MeshItem->GetRelativeRotation();
	MeshItem->SetRelativeRotation(Rotation + FRotator(0.f, 180.f * DeltaTime, 0.f));
}

void APMPItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!OtherActor->IsA(APMPCharacter::StaticClass()))
		return;

	APMPCharacter* OverlappedCharacter = Cast<APMPCharacter>(OtherActor);
	
	UNiagaraComponent* NiagaraComponent;
	
	if (eItemType == EItemType::ITEM_BOOST)
	{
		OverlappedCharacter->SetBoost();
		
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraBoost, 
		OverlappedCharacter->GetMesh(),
		FName(TEXT("Niagara")),
		FVector(0.0f, 0.0f, 70.0f),
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
	}
	else if (eItemType == EItemType::ITEM_STRENGTHEN)
	{
		OverlappedCharacter->SetStrengthen();
		
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraStrengthen, 
		OverlappedCharacter->GetMesh(),
		FName(TEXT("Niagara")),
		FVector(0.0f, 0.0f, 70.0f),
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
	}
	else if (eItemType == EItemType::ITEM_INVINCIBLE)
	{
		OverlappedCharacter->SetInvincible();
		
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraInvincible, 
		OverlappedCharacter->GetMesh(),
		FName(TEXT("Niagara")),
		FVector(0.0f, 0.0f, 70.0f),
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
	}
	
	Destroy();
}

