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
	
	UPROPERTY(EditAnywhere)	
	class APMPAIController* PMPAIController;

	
protected:
	UPROPERTY()
	int32 Damage;
	UPROPERTY()
	int32 MaxHP;
	UPROPERTY()
	int32 CurHP;
	UPROPERTY()
	int32 TakenDamage;
	
public:
	UFUNCTION()
	int32 GetDamage() const { return Damage; };
	
	UFUNCTION(BlueprintCallable)
	int32 GetTakenDamage() const { return TakenDamage; };
	
	UFUNCTION(BlueprintCallable)
	int32 GetMaxHP() const { return MaxHP; };
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurHP() const { return CurHP; };
	
	UFUNCTION()	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TakeDamage")
	void OnTakeDamageExecuted();
};
