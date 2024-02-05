// Copyright Epic Games, Inc. All Rights Reserved.

#include "PMPGameMode.h"
#include "PMPCharacterAurora.h"
#include "PMPCharacterMuriel.h"
#include "PMPPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

APMPGameMode::APMPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> BPAurora(TEXT("Blueprint'/Game/Player/BP_PMPCharacterAurora.BP_PMPCharacterAurora_C'"));
	CharacterAuroraClass = BPAurora.Class;
	
	static ConstructorHelpers::FClassFinder<APawn> BPMuriel(TEXT("Blueprint'/Game/Player/BP_PMPCharacterMuriel.BP_PMPCharacterMuriel_C'"));
	CharacterMurielClass = BPMuriel.Class;
	
}

void APMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NewPlayerController = Cast<APMPPlayerController>(NewPlayer);
	if (NewPlayerController->GetPlayerState<APlayerState>() && (NewPlayerController->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 0)
	{		
		if (CharacterAuroraClass != NULL)
		{
			APMPCharacterAurora* NewCharacter = GetWorld()->SpawnActor<APMPCharacterAurora>(CharacterAuroraClass, FVector(0.f, -100.0f, 96.0f), FRotator());
			NewPlayerController->Possess(NewCharacter);
			UE_LOG(LogTemp, Warning, TEXT("login 1p"));
		}
	}
	else if (NewPlayerController->GetPlayerState<APlayerState>() && (NewPlayerController->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 1)
	{
		if (CharacterMurielClass != NULL)
		{
			APMPCharacterMuriel* NewCharacter = GetWorld()->SpawnActor<APMPCharacterMuriel>(CharacterMurielClass, FVector(0.f, 100.0f, 96.0f), FRotator());
			NewPlayerController->Possess(NewCharacter);
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
