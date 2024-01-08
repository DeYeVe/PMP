// Copyright Epic Games, Inc. All Rights Reserved.

#include "PMPGameMode.h"
#include "PMPCharacter.h"
#include "PMPCharacterAurora.h"
#include "PMPCharacterFey.h"
#include "PMPUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

APMPGameMode::APMPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> BPAurora(TEXT("Blueprint'/Game/Player/BP_PMPCharacterAurora.BP_PMPCharacterAurora_C'"));
	CharacterAuroraClass = BPAurora.Class;
	
	static ConstructorHelpers::FClassFinder<APawn> BPFey(TEXT("Blueprint'/Game/Player/BP_PMPCharacterFey.BP_PMPCharacterFey_C'"));
	CharacterFeyClass = BPFey.Class;

	// hud	
	static ConstructorHelpers::FClassFinder<UPMPUserWidget> WidgetClassFinder(TEXT("'/Game/UI/WBP_Widget.WBP_Widget_C'"));

	if(WidgetClassFinder.Succeeded())
	{
		Widget =  WidgetClassFinder.Class;
		CurWidget = CreateWidget(GetWorld(), Widget);
		if(CurWidget)
		{
			CurWidget->AddToViewport();
		}
	}
}

void APMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (NewPlayer->GetPlayerState<APlayerState>() && (NewPlayer->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 0)
	{		
		if (CharacterAuroraClass != NULL)
		{
			APMPCharacterAurora* NewCharacter = GetWorld()->SpawnActor<APMPCharacterAurora>(CharacterAuroraClass, FVector(0.f, -100.0f, 96.0f), FRotator());
			NewPlayer->Possess(NewCharacter);
			UE_LOG(LogTemp, Warning, TEXT("login 1p"));
		}
	}
	else if (NewPlayer->GetPlayerState<APlayerState>() && (NewPlayer->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 1)
	{
		if (CharacterFeyClass != NULL)
		{
			APMPCharacterFey* NewCharacter = GetWorld()->SpawnActor<APMPCharacterFey>(CharacterFeyClass, FVector(0.f, 100.0f, 96.0f), FRotator());
			NewPlayer->Possess(NewCharacter);
			UE_LOG(LogTemp, Warning, TEXT("login 2p"));
		}		
	}
}

void APMPGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APMPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
