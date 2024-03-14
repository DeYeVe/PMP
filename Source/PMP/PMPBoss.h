// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PMPBoss.generated.h"

class APMPProjectile;

UCLASS()
class PMP_API APMPBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APMPBoss();

protected:	
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	USkeletalMeshComponent* MeshBoss;
	
public:	
	UPROPERTY(EditAnywhere)
	class UPMPAnimInstance* AnimInstance;
	
	USkeletalMeshComponent* GetMesh() const { return MeshBoss; }

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
	class APMPAIControllerBoss* PMPAIController;

protected:
	UPROPERTY(EditAnywhere, Category = "Boss Stats")
	int32 Damage;
	UPROPERTY(EditAnywhere, Category = "Boss Stats")
	int32 MaxHP;
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditAnywhere, Category = "Boss Stats")
	int32 CurHP;
	UFUNCTION(BlueprintCallable)
	virtual void OnRep_HP(int32 LastHP);
	UPROPERTY(Replicated)
	int32 TakenDamage;
	UPROPERTY()
	float MoveSpeed = 600.f;
	
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
	void Attack();
	UFUNCTION()
	void LocalAttack();
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();
	UFUNCTION()
	void CheckAttack();

	UFUNCTION()
	void Skill_1();
	UFUNCTION()
	void LocalSkill_1();
	UFUNCTION(Server, Reliable)
	void ServerSkill_1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_1();
	UFUNCTION()
	void SpawnSkill_1();
	
	UFUNCTION()
	void Skill_2();
	UFUNCTION()
	void LocalSkill_2();
	UFUNCTION(Server, Reliable)
	void ServerSkill_2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_2();
	UFUNCTION()
	void SpawnSkill_2();
	int32 Skill2Offset;
	
	UFUNCTION()
	void Skill_3();
	UFUNCTION()
	void LocalSkill_3();
	UFUNCTION(Server, Reliable)
	void ServerSkill_3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_3();
	UFUNCTION()
	void SpawnSkill_3();
	UFUNCTION()
	void GenerateRandomLocationAndSync();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncRandomLocation(const FVector& RandomLocation);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SyncRandomLocation(APlayerController* PlayerController, const FVector& RandomLocation);

private:
	UPROPERTY(VisibleAnywhere)
	UParticleSystem* Skill_3FX;
	UPROPERTY(VisibleAnywhere)
	UParticleSystem* Skill_4FX;
	UPROPERTY(VisibleAnywhere)
	UParticleSystem* DieFX;
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	TSubclassOf<class ADecalActor> Skill3DecalClass;

public:
	UFUNCTION()
	void Skill_4();
	UFUNCTION()
	void LocalSkill_4();
	UFUNCTION(Server, Reliable)
	void ServerSkill_4();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSkill_4();
	UFUNCTION()
	void SpawnSkill_4();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnProjectile(TSubclassOf<APMPProjectile> ProjectileClass, const FTransform& SpawnTransform, int32 ProjectileDamage);

	UFUNCTION()
	void Die();
	UFUNCTION()
	void LocalDie();
	UFUNCTION(Server, Reliable)
	void ServerDie();	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDie();
	
public:	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APMPProjectile> Skill1ProjectileClass;
	
	UFUNCTION()	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "TakeDamage")
	void OnTakeDamageExecuted();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TakeDamage")
	void OnInRange();

	UPROPERTY(Replicated)
	bool IsActing;
	
	UPROPERTY()
	bool CanAct = true;
	UFUNCTION()
	bool GetCanAct() const { return CanAct; };

	UPROPERTY()
	class APMPCharacter* Target;
	UFUNCTION()
	void SearchTarget();
	
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
