// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckCanAct.generated.h"

/**
 * 
 */
UCLASS()
class PMP_API UBTService_CheckCanAct : public UBTService
{
	GENERATED_BODY()

	UBTService_CheckCanAct();
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
