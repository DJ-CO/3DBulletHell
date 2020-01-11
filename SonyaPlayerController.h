// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SonyaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SONYAWITCHES_API ASonyaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASonyaPlayerController();

protected:
	// APawn interface
	virtual void SetupInputComponent() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void OnJumpPressed();
	void OnJumpReleased();
	void OnCrouchPressed();
	void OnCrouchReleased();

	bool bIsJumpPressed;
	bool bIsCrouchPressed;

	float YValue;
	float XValue;
public:
	float GetXValue();
	float GetYValue();
	FVector GetDirection();
	FVector GetXYDirection();
	bool GetIsJumpPressed();
	bool GetIsCrouchPressed();
};
