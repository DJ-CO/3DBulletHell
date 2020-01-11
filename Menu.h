// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ButtonComponent.h"
#include "Camera/CameraComponent.h"
#include "SonyaWitchesGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Menu.generated.h"

UCLASS()
class SONYAWITCHES_API AMenu : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// One button
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	// One button
	UPROPERTY(EditAnywhere)
	class UButtonComponent *MyButton;

	// The Buttons
	class UButtonComponent *MyButtons [2];

	// The mesh
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ScreenMesh;

	class UButtonComponent* SelectedButton;

	int CurrentButtonIndex;
	void CycleButtons();
	void GoToFileSelect();
	void ExitGame();
	void TestSwitchingLevels();


	// Gamemode
	class ASonyaWitchesGameMode* GameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGameMode(class ASonyaWitchesGameMode* NewMode);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
