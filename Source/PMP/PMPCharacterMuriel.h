// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPCharacter.h"
#include "PMPCharacterMuriel.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPCharacterMuriel : public APMPCharacter
{
	GENERATED_BODY()

public:
	APMPCharacterMuriel();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
};
