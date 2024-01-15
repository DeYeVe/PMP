// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

APMPAIController::APMPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Enemies/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		EnemyBehaviorTree = BT.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/Enemies/AI/BB_Enemy.BB_Enemy'"));
	if (BD.Succeeded())
	{
		EnemyBlackboardData = BD.Object;
	}
}

void APMPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if(UseBlackboard(EnemyBlackboardData, BlackboardComp))
	{
		if(RunBehaviorTree(EnemyBehaviorTree))
		{
		}
	}
	this->Blackboard = BlackboardComp;
}

void APMPAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
