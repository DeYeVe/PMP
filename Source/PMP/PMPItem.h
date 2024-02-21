// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PMPItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ITEM_BOOST,
	ITEM_INVINCIBLE,
	ITEM_STRENGTHEN,	
};

UCLASS()
class PMP_API APMPItem : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category=Mesh)
	class UStaticMeshComponent* MeshItem;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, Category=Item)
	TEnumAsByte<EItemType> eItemType;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraBoost;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraStrengthen;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraInvincible;
	
public:	
	// Sets default values for this actor's properties
	APMPItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
