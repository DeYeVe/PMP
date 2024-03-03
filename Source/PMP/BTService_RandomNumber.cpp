// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomNumber.h"

#include "PMPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_RandomNumber::UBTService_RandomNumber()
{
	NodeName = TEXT("CreateRandomNumber");

	bCallTickOnSearchStart = true;
	
}

void UBTService_RandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	int x = 0;
	x = FMath::RandRange(1, 100);

	if (x <= 40)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("SelectAttackNumber")), 0);
	}
	else if (x <= 55)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("SelectAttackNumber")), 1);
	}
	else if (x <= 60)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("SelectAttackNumber")), 2);
	}
	else if (x <= 85)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("SelectAttackNumber")), 3);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("SelectAttackNumber")), 4);
	}
}