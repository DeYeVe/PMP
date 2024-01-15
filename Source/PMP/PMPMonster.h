// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PMPMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

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
	
	USkeletalMeshComponent* GetMesh() const { return MeshMonster; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere)	
	class APMPAIController* PMPAIController;

	
protected:
	UPROPERTY(EditAnywhere, Category = "Monster Stats")
	int32 Damage;
	UPROPERTY(EditAnywhere, Category = "Monster Stats")
	int32 MaxHP;
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditAnywhere, Category = "Monster Stats")
	int32 CurHP;
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_HP(int32 LastHP);
	UPROPERTY(Replicated)
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

public:
	UFUNCTION()
	virtual void Hit();
	UFUNCTION()
	virtual void Attack();
	UFUNCTION()
	virtual void Die();
	
	UFUNCTION()	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TakeDamage")
	void OnTakeDamageExecuted();

	UPROPERTY(Replicated)
	bool IsActing;
	
	FOnAttackEnd OnAttackEnd;
	
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
