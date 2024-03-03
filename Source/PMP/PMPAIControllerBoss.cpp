// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAIControllerBoss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APMPAIControllerBoss::APMPAIControllerBoss()
{
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Enemies/AI/BT_Boss.BT_Boss'"));
	if (BT.Succeeded())
	{
		BossBehaviorTree = BT.Object;
	}
	
	const ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/Enemies/AI/BB_Boss.BB_Boss'"));
	if (BD.Succeeded())
	{
		BossBlackboardData = BD.Object;
	}
}

void APMPAIControllerBoss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if(UseBlackboard(BossBlackboardData, BlackboardComp))
	{
		if(RunBehaviorTree(BossBehaviorTree))
		{
		}
	}
	this->Blackboard = BlackboardComp;
}

void APMPAIControllerBoss::OnUnPossess()
{
	Super::OnUnPossess();
}
