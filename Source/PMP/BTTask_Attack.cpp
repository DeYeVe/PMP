// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "PMPAIController.h"
#include "PMPMonster.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto PMPMonster = Cast<APMPMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (PMPMonster == nullptr)
		return EBTNodeResult::Failed;

	PMPMonster->Attack();
	bIsAttacking = true;


	PMPMonster->OnHitEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});
	PMPMonster->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}