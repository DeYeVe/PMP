// Copyright Epic Games, Inc. All Rights Reserved.

#include "PMPCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PMPAnimInstance.h"
#include "PMPGameMode.h"
#include "PMPMonster.h"
#include "PMPPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APMPCharacter

APMPCharacter::APMPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	/*// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;*/

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APMPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	AnimInstance = Cast<UPMPAnimInstance>(GetMesh()->GetAnimInstance());
}

void APMPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateHUDSkill();
	UpdateHUDBuff();

	if (EnumHasAnyFlags(eStatesFlag, EStateFlags::FOCUSING))
	{
		float SearchRadius = 1200.f;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams(NAME_None, false);

		bool bResult = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(SearchRadius),
			QueryParams);

		if (bResult)
		{
			if (!OverlapResults.IsEmpty())
			{
				AActor* TargetEnemy = nullptr;
				float MinDistance = 0.f;
            		
				for (auto& OverlapResult : OverlapResults)
				{
					if (TargetEnemy == nullptr)
					{
						TargetEnemy = OverlapResult.GetActor();
						MinDistance = GetDistanceTo(TargetEnemy);
					}
					else if (MinDistance > GetDistanceTo(OverlapResult.GetActor()))
					{
						TargetEnemy = OverlapResult.GetActor();
						MinDistance = GetDistanceTo(TargetEnemy);
					}
				}
				if(DEBUG_FLAG)	
					DrawDebugSphere(GetWorld(), GetActorLocation(), SearchRadius, 16, FColor::Green, false, 0.2f);
			
				// Focusing

				if (TargetEnemy == nullptr)
					return;
				
				FVector Target = TargetEnemy->GetActorLocation();
				//float Dist = GetDistanceTo(PMPMonster);
				//Target.Z -= Dist / FMath::Sqrt(3.f);
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetCameraBoom()->GetComponentLocation(), Target);
				FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), LookAtRotation, DeltaSeconds, 10.0f);
				GetController()->SetControlRotation(NewRotation);
				return;
			}
			if(DEBUG_FLAG)	
				DrawDebugSphere(GetWorld(), GetActorLocation(), SearchRadius, 16, FColor::Red, false, 0.2f);
		}
		else
		{
			if(DEBUG_FLAG)	
				DrawDebugSphere(GetWorld(), GetActorLocation(), SearchRadius, 16, FColor::Red, false, 0.2f);
		}
	}
}

void APMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APMPCharacter, CurHP);
	DOREPLIFETIME(APMPCharacter, eStatesFlag);
}

//////////////////////////////////////////////////////////////////////////
// Input

void APMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APMPCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APMPCharacter::Look);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APMPCharacter::Attack);
		
		// Skill 1
		EnhancedInputComponent->BindAction(SkillAction_1, ETriggerEvent::Triggered, this, &APMPCharacter::Skill_1);
		
		// Skill 2
		EnhancedInputComponent->BindAction(SkillAction_2, ETriggerEvent::Triggered, this, &APMPCharacter::Skill_2);

		// Skill 3
		EnhancedInputComponent->BindAction(SkillAction_3, ETriggerEvent::Triggered, this, &APMPCharacter::Skill_3);

		// Toggle Focusing
		EnhancedInputComponent->BindAction(ToggleFocusingAction, ETriggerEvent::Triggered, this, &APMPCharacter::ToggleFocusing);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APMPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APMPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APMPCharacter::OnRep_HP(int32 LastHP)
{
	int32 DeltaHP = LastHP - CurHP;
	if (DeltaHP > 0)
		OnTakeDamageExecuted(DeltaHP);
	else
		OnTakeHealExecuted(DeltaHP);
	UpdateHUDHP();
}

void APMPCharacter::UpdateHUDHP()
{
	PMPPlayerController = IsValid(PMPPlayerController) ? PMPPlayerController : Cast<APMPPlayerController>(Controller);
	if (PMPPlayerController)
	{
		PMPPlayerController->SetHUDHP(CurHP, MaxHP);
	}
}

void APMPCharacter::UpdateHUDSkill()
{
	PMPPlayerController = IsValid(PMPPlayerController) ? PMPPlayerController : Cast<APMPPlayerController>(Controller);
	if (PMPPlayerController)
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(SkillCooldownTimerHandles[0]);
		PMPPlayerController->SetHUDCooldown1(RemainingTime, SkillCooldown[0]);
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(SkillCooldownTimerHandles[1]);
		PMPPlayerController->SetHUDCooldown2(RemainingTime, SkillCooldown[1]);
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(SkillCooldownTimerHandles[2]);
		PMPPlayerController->SetHUDCooldown3(RemainingTime, SkillCooldown[2]);
	}
}

void APMPCharacter::UpdateHUDBuff()
{
	PMPPlayerController = IsValid(PMPPlayerController) ? PMPPlayerController : Cast<APMPPlayerController>(Controller);
	if (PMPPlayerController)
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(BuffTimerHandles[0]);
		PMPPlayerController->SetHUDBuffBoost(RemainingTime, 8.f);
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(BuffTimerHandles[1]);
		PMPPlayerController->SetHUDBufStrengthen(RemainingTime, 8.f);
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(BuffTimerHandles[2]);
		PMPPlayerController->SetHUDBuffInvincible(RemainingTime, 8.f);
	}
}

void APMPCharacter::Attack()
{
}

void APMPCharacter::Skill_1()
{	
}

void APMPCharacter::Skill_2()
{
}

void APMPCharacter::Skill_3()
{
}

void APMPCharacter::StartSkillCooldown(int32 SkillIndex)
{
	auto OnSkillCooldownFinishedLambda = [this, SkillIndex]()
	{
		IsSkillOnCooldown[SkillIndex] = false;
	};
	
	GetWorldTimerManager().SetTimer(SkillCooldownTimerHandles[SkillIndex], OnSkillCooldownFinishedLambda, SkillCooldown[SkillIndex], false);
	IsSkillOnCooldown[SkillIndex] = true;
}

void APMPCharacter::ToggleFocusing()
{
	UE_LOG(LogTemp, Warning, TEXT("toggle focusing"));
	eStatesFlag ^= EStateFlags::FOCUSING;
}

float APMPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                AActor* DamageCauser)
{
	if (HasAuthority())
		ServerTakeDamage(DamageAmount);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APMPCharacter::ServerTakeDamage_Implementation(float DamageAmount)
{
	if (EnumHasAnyFlags(eStatesFlag, EStateFlags::INVINCIBLE))
		DamageAmount = 0.f;

	float LastHP = CurHP;
	CurHP -= DamageAmount;
	CurHP = CurHP > MaxHP ? MaxHP : CurHP;
	
	float DeltaHP = LastHP - CurHP;
	if (DeltaHP != 0.f)
	{
		if (DeltaHP > 0)
			OnTakeDamageExecuted(DeltaHP);
		else
			OnTakeHealExecuted(DeltaHP);
		UpdateHUDHP();		
	}
}

void APMPCharacter::SetBoost()
{	
	EnumAddFlags(eStatesFlag, EStateFlags::BOOST);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * 1.5;
	
	GetWorldTimerManager().SetTimer(BuffTimerHandles[0], this, &APMPCharacter::OnBoostReleased, 8.0f, false);
}

void APMPCharacter::OnBoostReleased()
{
	EnumRemoveFlags(eStatesFlag, EStateFlags::BOOST);
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void APMPCharacter::SetStrengthen()
{	
	EnumAddFlags(eStatesFlag, EStateFlags::STRENGTHENED);

	Damage = int32(float(DefaultDamage) * 1.5f);
	
	GetWorldTimerManager().SetTimer(BuffTimerHandles[1], this, &APMPCharacter::OnStrengthenReleased, 8.0f, false);
}

void APMPCharacter::OnStrengthenReleased()
{    
	EnumRemoveFlags(eStatesFlag, EStateFlags::STRENGTHENED);
	
	Damage = DefaultDamage;
}

void APMPCharacter::SetInvincible()
{
	EnumAddFlags(eStatesFlag, EStateFlags::INVINCIBLE);
	
	GetWorldTimerManager().SetTimer(BuffTimerHandles[2], this, &APMPCharacter::OnInvincibleReleased, 8.0f, false);

}

void APMPCharacter::OnInvincibleReleased()
{
	EnumRemoveFlags(eStatesFlag, EStateFlags::INVINCIBLE);
}

void APMPCharacter::SetSilence()
{
	EnumAddFlags(eStatesFlag, EStateFlags::SILENCED);

	PMPPlayerController = IsValid(PMPPlayerController) ? PMPPlayerController : Cast<APMPPlayerController>(Controller);
	if (PMPPlayerController)
	{
		PMPPlayerController->SetHUDSilence(true);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APMPCharacter::OnSilenceReleased, 5.f, false);
}

void APMPCharacter::OnSilenceReleased()
{
	EnumRemoveFlags(eStatesFlag, EStateFlags::SILENCED);

	PMPPlayerController = IsValid(PMPPlayerController) ? PMPPlayerController : Cast<APMPPlayerController>(Controller);
	if (PMPPlayerController)
	{
		PMPPlayerController->SetHUDSilence(false);
	}
}
