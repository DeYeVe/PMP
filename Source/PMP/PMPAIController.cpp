// Fill out your copyright notice in the Description page of Project Settings.


#include "PMPAIController.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

APMPAIController::APMPAIController()
{
}

void APMPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APMPAIController::MoveRandom, 5.f, true);
}

void APMPAIController::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void APMPAIController::MoveRandom()
{
	auto CurPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 600.f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this,RandomLocation);
	}
}
