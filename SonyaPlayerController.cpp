// Fill out your copyright notice in the Description page of Project Settings.


#include "SonyaPlayerController.h"
#include "Components/InputComponent.h"

ASonyaPlayerController::ASonyaPlayerController()
{
	XValue = 0;
	YValue = 0;
}

void ASonyaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	FInputActionBinding *ActionBinding;
	FInputAxisBinding *AxisBinding;

	ActionBinding = &InputComponent->BindAction("Jump", IE_Pressed, this, &ASonyaPlayerController::OnJumpPressed);
	ActionBinding->bConsumeInput = false;
	ActionBinding = &InputComponent->BindAction("Jump", IE_Released, this, &ASonyaPlayerController::OnJumpReleased);
	ActionBinding->bConsumeInput = false;

	ActionBinding = &InputComponent->BindAction("Crouch", IE_Pressed, this, &ASonyaPlayerController::OnCrouchPressed);
	ActionBinding->bConsumeInput = false;
	ActionBinding = &InputComponent->BindAction("Crouch", IE_Released, this, &ASonyaPlayerController::OnCrouchPressed);
	ActionBinding->bConsumeInput = false;

	AxisBinding = &InputComponent->BindAxis("MoveForward", this, &ASonyaPlayerController::MoveForward);
	AxisBinding->bConsumeInput = false;
	AxisBinding = &InputComponent->BindAxis("MoveRight", this, &ASonyaPlayerController::MoveRight);
	AxisBinding->bConsumeInput = false;
}

void ASonyaPlayerController::OnJumpPressed()
{
	bIsJumpPressed = true;
}

void ASonyaPlayerController::OnJumpReleased()
{
	bIsJumpPressed = false;
}

void ASonyaPlayerController::OnCrouchPressed()
{
	bIsCrouchPressed = true;
}

void ASonyaPlayerController::OnCrouchReleased()
{
	bIsCrouchPressed = false;
}

/** Called for forwards/backward input */
void ASonyaPlayerController::MoveForward(float Value)
{
	XValue = Value;
}

bool ASonyaPlayerController::GetIsJumpPressed()
{
	return bIsJumpPressed;
}

bool ASonyaPlayerController::GetIsCrouchPressed()
{
	return bIsCrouchPressed;
}


/** Called for side to side input */
void ASonyaPlayerController::MoveRight(float Value)
{
	YValue = Value;
}

FVector ASonyaPlayerController::GetDirection()
{
	return FVector(XValue, YValue, 1 - (XValue * XValue + YValue * YValue));
}

float ASonyaPlayerController::GetXValue()
{
	return XValue;
}

float ASonyaPlayerController::GetYValue()
{
	return YValue;
}

FVector ASonyaPlayerController::GetXYDirection()
{
	return FVector(XValue, YValue, 0.0f);
}
