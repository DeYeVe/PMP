// Copyright Epic Games, Inc. All Rights Reserved.

#include "PMPGameMode.h"
#include "PMPCharacterAurora.h"
#include "PMPCharacterMuriel.h"
#include "PMPPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
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
	if (NewPlayerController->GetPlayerState<APlayerState>() && (NewPlayerController->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 1)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APMPCharacterAurora::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			APMPCharacterAurora* CharacterAurora = Cast<APMPCharacterAurora>(FoundActors[0]);
			NewPlayerController->Possess(CharacterAurora);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("login 1p"));
	}
	else if (NewPlayerController->GetPlayerState<APlayerState>() && (NewPlayerController->GetPlayerState<APlayerState>()->GetPlayerId() - 256) % 2  == 0)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APMPCharacterMuriel::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			APMPCharacterMuriel* CharacterMuriel = Cast<APMPCharacterMuriel>(FoundActors[0]);
			NewPlayerController->Possess(CharacterMuriel);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("login 1p"))
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
