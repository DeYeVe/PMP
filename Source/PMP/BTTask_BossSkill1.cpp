// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossSkill1.h"

#include "AIController.h"
#include "PMPBoss.h"

UBTTask_BossSkill1::UBTTask_BossSkill1()
{
}

EBTNodeResult::Type UBTTask_BossSkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<APMPBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	Boss->Skill_1();

	return Result;
}
