// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PMPAIControllerBoss.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPAIControllerBoss : public AAIController
{
	GENERATED_BODY()

public:
	APMPAIControllerBoss();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class UBehaviorTree* BossBehaviorTree;

	UPROPERTY()
	class UBlackboardData* BossBlackboardData;	
	
};
