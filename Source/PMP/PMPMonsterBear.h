// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMPMonster.h"
#include "PMPMonsterBear.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPMonsterBear : public APMPMonster
{
	GENERATED_BODY()

public:
	APMPMonsterBear();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
