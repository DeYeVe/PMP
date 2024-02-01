// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PMPAIController.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API APMPAIController : public AAIController
{
	GENERATED_BODY()

	/**
	 * 
	 */
public:
	APMPAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
private:
	UPROPERTY()
	class UBehaviorTree* EnemyBehaviorTree;

	UPROPERTY()
	class UBlackboardData* EnemyBlackboardData;	
};
