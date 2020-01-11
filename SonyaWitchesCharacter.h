// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SonyaWitchesGameMode.h"
#include "SonyaPlayerController.h"
#include "SonyaAction.h"
#include "SonyaWitchesCharacter.generated.h"

UCLASS(config=Game)
class ASonyaWitchesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASonyaWitchesCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	ASonyaPlayerController * MyController;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handles jumping and ascending **/
	void Ascend();

	/** Handles descending **/
	void Descend();

	/** Handles crouch **/
	void StartCrouch();

	/** Handles uncrouch **/
	void EndCrouch();

	/** Tests Level Loading **/
	void TestLevelLoad();

	/** Charge jump **/
	void ChargeJump();

	/** Charge jump **/
	void ReleaseJump();

	void FlameSpiral();

	void SparkFantasia();

	void EmberDance();

	void Zoom(float Value);

	void QuickDash();

	bool InputWasQuickDash();

	void Sprint();

	void StopSprinting();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Need this to get world
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	// End of APawn interface

	FVector SpringFactor(FVector CurrentVelocity, FVector AddedVelocity);

	// Sonya Variables
	UPROPERTY(EditAnywhere)
	float AscendSpeed;

	UPROPERTY(EditAnywhere)
	float DescendSpeed;

	UPROPERTY(EditAnywhere)
	FVector RespawnLocation;

	UPROPERTY(EditAnywhere)
	float FlameSpiralHeight;

	UPROPERTY(EditAnywhere)
	float FlameSpiralDrift;

	UPROPERTY(EditAnywhere)
	float FlameSpiralFall;

	UPROPERTY(EditAnywhere)
	float FlameSpiralFallDrift;

	UPROPERTY(EditAnywhere)
	float FlameSpiralLag;

	UPROPERTY(EditAnywhere)
	float SparkFantasiaSpeed;

	UPROPERTY(EditAnywhere)
	float SparkFantasiaLag;

	UPROPERTY(EditAnywhere)
	float SparkFantasiaLock;

	UPROPERTY(EditAnywhere)
	float EmberDanceSpeed;

	UPROPERTY(EditAnywhere)
	float EmberDanceUpLag;

	UPROPERTY(EditAnywhere)
	float EmberDanceUpFactor;

	UPROPERTY(EditAnywhere)
	float WallJumpTolerance;

	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed;

	UPROPERTY(EditAnywhere)
	float MaxCameraDistance;

	UPROPERTY(EditAnywhere)
	float MinCameraDistance;

	UPROPERTY(EditAnywhere)
	float SprintSpeed;

	UPROPERTY(EditAnywhere)
	float JogSpeed;

	float CameraZoomPercent;

	float MovementLagTime;

	float VelocityLockTime;

	FVector LockedVelocity;

	int State;

	float JumpPower;

	float WallJumpSpeed;

	float WallSlideFallSpeed;

	bool IsChargingJump;

	int IsWallSliding;

	bool IsPushing;

	FVector WallDirection;

	float JumpSquatSpeed;

	float RunSpeed;

	float LowJumpSpeed;

	float MidJumpSpeed;

	float HighJumpSpeed;

	// Gamemode
	class ASonyaWitchesGameMode* GameMode;

protected:
	UPROPERTY(EditAnywhere)
	float TimeIdle;
	float TimeIdleThreshold;
	float QuickStartThreshold;
	float TimeSinceLastIdleInput;
	float QuickStartSpeed;
	FVector LastInput;

protected:
	UPROPERTY(EditAnywhere)
		int DoubleJumps;
	UPROPERTY(EditAnywhere)
		int AirDashes;

	int DoubleJumpsLeft;
	int AirDashesLeft;
	float DoubleJumpSpeed;

	// Actions
protected:
	void InitializeActions();
	/*
	class USonyaAction* Actions[NUM_ACTIONS];
	class USonyaJump MyJump;
	class USonyaCrouch MyCrouch;
	class USonyaCrouch MyMove;*/

public:
	ASonyaPlayerController * GetMyController();

	bool bIsPushingWall;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void Die();

	/** Handles Hits **/
	UFUNCTION()
	void OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

