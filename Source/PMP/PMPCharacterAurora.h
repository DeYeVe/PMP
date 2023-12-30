// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPCharacter.h"
#include "PMPCharacterAurora.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPCharacterAurora : public APMPCharacter
{
	GENERATED_BODY()

public:
	APMPCharacterAurora();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Attack() override;
	virtual void Skill_1() override;
	virtual void Skill_2() override;
	
};
