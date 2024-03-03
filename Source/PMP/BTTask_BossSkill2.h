// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill2.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API UBTTask_BossSkill2 : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_BossSkill2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
