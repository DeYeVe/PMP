// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossAttack.h"

#include "AIController.h"
#include "PMPBoss.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<APMPBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	Boss->Attack();

	return Result;
}
