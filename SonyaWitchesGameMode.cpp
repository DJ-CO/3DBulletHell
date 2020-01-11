// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SonyaWitchesGameMode.h"
#include "Menu.h"
#include "SonyaWitchesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "DummyContextObject.h"
#include "Engine/World.h"
#include "SonyaPlayerController.h"

ASonyaWitchesGameMode::ASonyaWitchesGameMode()
{
	// set default pawn class to our Blueprinted character
	PlayerControllerClass = ASonyaPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
}

void ASonyaWitchesGameMode::BeginPlay()
{
	Super::BeginPlay();
	//InitializeWorldData();
	//OnGameStart();
}

void ASonyaWitchesGameMode::GoToLevelSelect()
{

}

void ASonyaWitchesGameMode::GoToLevel(int Level, UWorld* TheWorld)
{
		
		/*
		ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(TheWorld, "HubWorld");
		switch (Level)
		{
		case 0:
			UGameplayStatics::OpenLevel(TheWorld, "Loading");
			break;

		case 1:
			pStreaming = UGameplayStatics::GetStreamingLevel(TheWorld, "HubWorld");
			UE_LOG(LogTemp, Warning, TEXT("In case"));
			if (true)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attempting stream"));
				bool temp = !(pStreaming->ShouldBeLoaded());
				pStreaming->SetShouldBeLoaded(temp);
				pStreaming->SetShouldBeVisible(temp);
				pStreaming->SetIsRequestingUnloadAndRemoval(false);
			}
			break;
		default:
			break;
		}
		*/

		FString CurrentLevel = TheWorld->GetMapName();
		
		if (CurrentLevel == "ThirdPersonExampleMap")
		{
			UGameplayStatics::OpenLevel(TheWorld, "HubWorld");
		}
		else
		{
			UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap");
		}
}

void ASonyaWitchesGameMode::OnGameStart()
{	
	AMenu* TitleScreen = NULL;
	ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(ContextObject, "TitleScreen");
	if (pStreaming)
	{
		pStreaming->SetShouldBeLoaded(true);
		pStreaming->SetShouldBeVisible(true);
	}

	if (World)
	{
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		TitleScreen = World->SpawnActor<AMenu>(Location, Rotation, SpawnInfo);
		TitleScreen->SetGameMode(this);
	}
	
	if (Controller && TitleScreen)
	{
		Controller->UnPossess();
		Controller->Possess(TitleScreen);
	}
	
}

void ASonyaWitchesGameMode::InitializeWorldData()
{
	World = GetWorld();
	
	if (World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got in here"));
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		ContextObject = World->SpawnActor<ADummyContextObject>(Location, Rotation, SpawnInfo);
	}

	ContextObject->SetActorHiddenInGame(true);
	ContextObject->SetActorEnableCollision(false);
	ContextObject->SetActorTickEnabled(false);
	
	Controller = UGameplayStatics::GetPlayerController(ContextObject, 0);
}

void ASonyaWitchesGameMode::LoadLevel(FName LevelToLoad)
{
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;

		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);

		ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(ContextObject, LevelToLoad);
		if (pStreaming)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actually Loaded"));
			pStreaming->SetShouldBeLoaded(true);
			pStreaming->SetShouldBeVisible(true);
			//pStreaming->SetIsRequestingUnloadAndRemoval(false);
		}
	}
}

void ASonyaWitchesGameMode::UnloadLevel(FName LevelToUnload)
{
	if (LevelToUnload != "")
	{
		FLatentActionInfo LatentInfo;

		ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(ContextObject, LevelToUnload);
		if (pStreaming)
		{
			UE_LOG(LogTemp, Warning, TEXT("Was there before unload"));
			pStreaming->SetShouldBeLoaded(false);
			pStreaming->SetShouldBeVisible(false);
			//pStreaming->SetIsRequestingUnloadAndRemoval(true);
		}
		
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, false);
		
	}
}

