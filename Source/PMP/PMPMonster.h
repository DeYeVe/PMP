// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PMPMonster.generated.h"

UCLASS()
class PMP_API APMPMonster : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APMPMonster();

protected:	
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	USkeletalMeshComponent* MeshMonster;
	
public:	
	UPROPERTY(EditAnywhere)
	class UPMPAnimInstance* AnimInstance;
	
	USkeletalMeshComponent* GetMeshMonster() const { return MeshMonster; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	int Damage;
	int MaxHP;
	int CurHP;
	
public:
	UFUNCTION()
	int GetDamage() const { return Damage; };
	
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere)	
	class APMPAIController* PMPAIController;
};
