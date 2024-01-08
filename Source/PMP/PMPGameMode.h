// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PMPGameMode.generated.h"

UCLASS(minimalapi)
class APMPGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	TSubclassOf<APawn> CharacterAuroraClass;
	TSubclassOf<APawn> CharacterFeyClass;
	
public:
	APMPGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> Widget;
	UPROPERTY()
	UUserWidget* CurWidget;

};



