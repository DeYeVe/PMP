// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PMPGameMode.generated.h"

#define DEBUG_FLAG false

UCLASS(minimalapi)
class APMPGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	TSubclassOf<APawn> CharacterAuroraClass;
	TSubclassOf<APawn> CharacterMurielClass;
	UPROPERTY()
	class APMPPlayerController* NewPlayerController;
	
public:
	APMPGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};



