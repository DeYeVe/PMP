// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PMPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStateFlags : uint8
{
	ACTING = 1 << 1,
	STRENGTHENED = 1 << 2,
	BOOST = 1 << 3,
	SILENCED = 1 << 4,
	SLOW = 1 << 5,
	INVINCIBLE = 1 << 6,
	FOCUSING = 1 << 7,
	TRASH = 0
};
ENUM_CLASS_FLAGS(EStateFlags);

UCLASS(config=Game)
class APMPCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))	
	UInputAction* LookAction;
	
	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	/** Skill1 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillAction_1;
	
	/** Skill2 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillAction_2;
	
	/** Skill3 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillAction_3;
	
	/** Toggle Focusing Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleFocusingAction;
	
	
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	USkeletalMeshComponent* MeshCharacter;
	
	UPROPERTY()
	UMaterialInterface* OriginalMaterial;

public:
	APMPCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	USkeletalMeshComponent* GetMesh() const { return MeshCharacter; }

protected:
	UPROPERTY()
	FVector2D MovementVector;

	UPROPERTY()
	class UPMPAnimInstance* AnimInstance;

	UPROPERTY()
	class APMPPlayerController* PMPPlayerController;
public:
	FVector2D GetMovementVector() const { return MovementVector; }
	
protected:

	UPROPERTY()
	EStateFlags eStatesFlag;
	UPROPERTY(ReplicatedUsing = OnRep_HP, EditAnywhere, Category = "Player Stats")
	int32 CurHP;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 MaxHP;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 Damage;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 DefaultDamage;
	UFUNCTION(BlueprintCallable)
	void OnRep_HP(int32 LastHP);
	UPROPERTY(Replicated)
	int32 TakenDamage;
	UPROPERTY()
	float MoveSpeed = 600.f;

	
public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurHP() const { return CurHP; };	
	UFUNCTION()
	void SetCurHP(int32 HP) { CurHP = HP; };
	UFUNCTION(BlueprintCallable)
	int32 GetMaxHP() const { return MaxHP; };
	UFUNCTION(BlueprintCallable)
	int32 GetDamage() const { return Damage; };
	
	UFUNCTION(BlueprintCallable)
	int32 GetTakenDamage() const { return TakenDamage; };

public:
	void UpdateHUDHP();
	void UpdateHUDSkill();
	void UpdateHUDBuff();

public:
	UFUNCTION()
	virtual void Attack();
	
	UFUNCTION()
	virtual void Skill_1();
	
	UFUNCTION()
	virtual void Skill_2();
	
	UFUNCTION()
	virtual void Skill_3();
	
	FTimerHandle SkillCooldownTimerHandles[3];
	bool IsSkillOnCooldown[3];
	float SkillCooldown[3];
	
	FTimerHandle BuffTimerHandles[3];

	UFUNCTION()
	virtual void StartSkillCooldown(int32 SkillIndex);
	
	UFUNCTION()
	virtual void ToggleFocusing();

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void SetBoost();
	UFUNCTION()
	virtual void OnBoostReleased();
	
	UFUNCTION()
	void SetStrengthen();
	UFUNCTION()
	virtual void OnStrengthenReleased();
	
	UFUNCTION()
	void SetInvincible();
	UFUNCTION()
	virtual void OnInvincibleReleased();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TakeDamage")
	void OnTakeDamageExecuted();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TakeHeal")
	void OnTakeHealExecuted();
};

