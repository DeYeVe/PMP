// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPCharacter.h"
#include "PMPCharacterFey.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPCharacterFey : public APMPCharacter
{
	GENERATED_BODY()

public:
	APMPCharacterFey();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
};
