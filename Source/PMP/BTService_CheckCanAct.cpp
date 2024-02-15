// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckCanAct.h"

#include "AIController.h"
#include "PMPMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckCanAct::UBTService_CheckCanAct()
{
	NodeName = TEXT("CheckCanAct");
	Interval = 0.1f;
}

void UBTService_CheckCanAct::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	if (CurrentPawn->IsA(APMPMonster::StaticClass()))
	{
		if (Cast<APMPMonster>(CurrentPawn)->GetCanAct())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanAct")), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanAct")), false);			
		}
	}
}
