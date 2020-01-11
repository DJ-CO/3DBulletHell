// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DummyContextObject.h"
#include "SonyaWitchesGameMode.generated.h"

UCLASS(minimalapi)
class ASonyaWitchesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASonyaWitchesGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* The controller at index 0 */
	class APlayerController* Controller;

	/* The World */
	class UWorld* World;

	/* A World Context Object*/
	class ADummyContextObject* ContextObject;

	/* Sends player to level select */
	void GoToLevelSelect();

	void OnGameStart();

	/* Sends player to level */
	void GoToLevel(int Level, UWorld* TheWorld);

	void LoadLevel(FName LevelToLoad);

	void UnloadLevel(FName LevelToUnload);

	void InitializeWorldData();
};



