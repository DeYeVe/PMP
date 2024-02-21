 // Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "PMPAIController.h"
#include "PMPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "PMPGameMode.h"

 UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.0f;
}
void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;
	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 800.f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		if (!OverlapResults.IsEmpty())
		{
			APMPCharacter* PMPCharacter = nullptr;
			float MinDistance = 0.f;
            		
			for (auto& OverlapResult : OverlapResults)
			{
				if (!(OverlapResult.GetActor()->IsA(APMPCharacter::StaticClass())))
					continue;
				
				if (PMPCharacter == nullptr)
				{
					PMPCharacter = Cast<APMPCharacter>(OverlapResult.GetActor());
					MinDistance = CurrentPawn->GetDistanceTo(PMPCharacter);
				}
				else if (MinDistance > CurrentPawn->GetDistanceTo(Cast<APMPCharacter>(OverlapResult.GetActor())))
				{
					PMPCharacter = Cast<APMPCharacter>(OverlapResult.GetActor());
					MinDistance = CurrentPawn->GetDistanceTo(PMPCharacter);
				}
			}
			if(DEBUG_FLAG)	
				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
			
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), PMPCharacter);

			return;
		}
		if(DEBUG_FLAG)	
			DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		if(DEBUG_FLAG)	
			DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
}
