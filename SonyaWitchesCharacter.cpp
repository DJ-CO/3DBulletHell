// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SonyaWitchesCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "SonyaPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ASonyaWitchesCharacter

ASonyaWitchesCharacter::ASonyaWitchesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ASonyaWitchesCharacter::OnCompHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASonyaWitchesCharacter::OnOverlapBegin);

	//InitializeActions();

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	JumpSquatSpeed = 0.0f;
	RunSpeed = 400.0f;
	SprintSpeed = 1200.0f;
	JogSpeed = 600.0f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->bUseSeparateBrakingFriction = false;
	GetCharacterMovement()->BrakingFriction = 0.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.00f;
	GetCharacterMovement()->BrakingDecelerationFalling = 0.0f;
	GetCharacterMovement()->BrakingDecelerationFlying = 0.0f;
	GetCharacterMovement()->FallingLateralFriction = 0.0f;
	GetCharacterMovement()->MaxAcceleration = JogSpeed*2;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GravityScale = 3.5;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LowJumpSpeed = 828.3;

	MidJumpSpeed = 1171.3;

	HighJumpSpeed = 1424.6;

	// Sonya Variables
	AscendSpeed = 1;
	DescendSpeed = 500;
	FlameSpiralHeight = 600.0f;
	FlameSpiralDrift = 600.0f;
	FlameSpiralFall = -2000.0f;
	FlameSpiralFallDrift = RunSpeed;
	FlameSpiralLag = 0.0;
	MovementLagTime = 0.0f;
	SparkFantasiaSpeed = 3000.0f;
	SparkFantasiaLag = 0.4f;
	SparkFantasiaLock= 0.2f;
	VelocityLockTime = 0.0f;
	LockedVelocity = FVector(0.0f);
	EmberDanceSpeed = 400.0f;
	EmberDanceUpLag = 0.7f;
	EmberDanceUpFactor = 0.7f;
	IsWallSliding = 0;
	WallDirection = FVector(0.0f);
	WallJumpTolerance = 100.0f;
	WallJumpSpeed = 1000;
	WallSlideFallSpeed = -300.0f;
	QuickStartThreshold = 0.5;
	TimeIdleThreshold = 0.05;
	QuickStartSpeed = 1100;
	DoubleJumps = 0;
	DoubleJumpsLeft = 0;
	AirDashes = 1;
	AirDashesLeft = 1;
	DoubleJumpSpeed = 650;
	CameraZoomSpeed = 0.03f;
	MaxCameraDistance = 100.0f;
	MinCameraDistance = 1000.0f;
	CameraZoomPercent = 0.5f;

	// Set tick for certain events
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ASonyaWitchesCharacter::InitializeActions()
{
	/*for (int i = 0; i < NUM_ACTIONS; i++)
	{
		switch (i)
		{
		case JUMP:
			Actions[JUMP] = &MyJump;
			Actions[JUMP]->Sonya = this;
			Actions[JUMP]->Controller = MyController;
		default:
			break;
		}
	}*/
}

void ASonyaWitchesCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = (ASonyaWitchesGameMode*)GetWorld()->GetAuthGameMode();

	RespawnLocation = GetActorLocation();

	if (Controller != NULL)
	{
		MyController = Cast<ASonyaPlayerController>(Controller);
	}
}

void ASonyaWitchesCharacter::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	int TickNeed = 0;

	//for (int i = 0; i < NUM_ACTIONS; i++)
	//{
	//	if (Actions[i]->bIsActing)
	//	{
	//		Actions[i]->DoAction(DeltaTime);
	//	}
	//}

	if (MyController->GetXYDirection().IsNearlyZero())
	{
		TickNeed++;
		TimeIdle += DeltaTime;

		if (TimeIdle > TimeIdleThreshold)
		{
			TimeSinceLastIdleInput = 0.0f;
		}
	}
	else
	{
		TickNeed++;

		TimeSinceLastIdleInput += DeltaTime;
		TimeIdle = 0.0f;
	}

	if (IsChargingJump)
	{
		TickNeed++;
		//UE_LOG(LogTemp, Warning, TEXT("Charging jump"));
		JumpPower += 5.0*DeltaTime;
		if (JumpPower > 1.0)
		{
			JumpPower = 1.0;
			IsChargingJump = false;
			ReleaseJump();
		}
	}

	if (MovementLagTime > 0.0f)
	{
		TickNeed++;
		MovementLagTime -= DeltaTime;
	}

	if (VelocityLockTime > 0.0f)
	{
		TickNeed++;
		VelocityLockTime -= DeltaTime;
		GetMovementComponent()->Velocity = LockedVelocity;

		if (VelocityLockTime < 0.01f)
		{
			LockedVelocity = FVector(0.0f);
			GetMovementComponent()->Velocity = FVector(0.0f);
		}
	}

	if (IsWallSliding)
	{
		TickNeed++;
		IsWallSliding--;
		GetMovementComponent()->Velocity.X = 0;
		GetMovementComponent()->Velocity.Y = 0;
		if (GetMovementComponent()->Velocity.Z < WallSlideFallSpeed)
		{
			GetMovementComponent()->Velocity.Z = WallSlideFallSpeed;
		}
	}

	if (IsPushing)
	{
		TickNeed++;
		IsPushing = false;
	}
	
	if (TickNeed == 0)
	{
		SetActorTickEnabled(false);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASonyaWitchesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASonyaWitchesCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASonyaWitchesCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASonyaWitchesCharacter::ChargeJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASonyaWitchesCharacter::ReleaseJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASonyaWitchesCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASonyaWitchesCharacter::StopSprinting);

	//PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ASonyaWitchesCharacter::SparkFantasia);

	//PlayerInputComponent->BindAction("Descend", IE_Pressed, this, &ASonyaWitchesCharacter::Descend);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASonyaWitchesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASonyaWitchesCharacter::MoveRight);

	PlayerInputComponent->BindAxis("ZoomIn", this, &ASonyaWitchesCharacter::Zoom);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASonyaWitchesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASonyaWitchesCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASonyaWitchesCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASonyaWitchesCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASonyaWitchesCharacter::OnResetVR);
}


void ASonyaWitchesCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASonyaWitchesCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASonyaWitchesCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASonyaWitchesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASonyaWitchesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASonyaWitchesCharacter::MoveForward(float Value)
{
	if (MyController != NULL)
	{
		Value = MyController->GetXValue();
	}
	
	if ((MyController != NULL) && (MovementLagTime < 0.01f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector Direction = YawRotation.RotateVector(MyController->GetXYDirection());
		//GetMovementComponent()->Velocity = SpringFactor(GetMovementComponent()->Velocity, Direction * RunSpeed);
		
		//if (TimeSinceLastIdleInput < QuickStartThreshold && Direction.Size() > 0.99)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Did quick start"));
		//	//GetMovementComponent()->Velocity += Direction * RunSpeed * 1.0;
		//	GetMovementComponent()->Velocity.X = Direction.X * RunSpeed * 1.2;
		//	GetMovementComponent()->Velocity.Y = Direction.Y * RunSpeed * 1.2;
		//}


		if (Direction.Size() > 0.0f)
		{
			AddMovementInput(Direction, Direction.Size());
		}
		
		LastInput = MyController->GetXYDirection();
	}

	SetActorTickEnabled(true);
}

void ASonyaWitchesCharacter::MoveRight(float Value)
{
	if (MyController != NULL)
	{
		Value = MyController->GetYValue();
	}

	if ( (MyController != NULL) && (MovementLagTime < 0.01f) && false)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		/*FVector Direction = YawRotation.RotateVector(MyController->GetDirection());
		Direction.Z = 0;
		Direction = RunSpeed * Direction;
		GetMovementComponent()->Velocity.X = Direction.X;
		GetMovementComponent()->Velocity.Y = Direction.Y;*/
		AddMovementInput(Direction, Value);
	}
}

void ASonyaWitchesCharacter::Ascend()
{
	if ((MyController != NULL))
	{
		// find out which way is forward
		const FRotator Rotation = MyController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get up vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		Direction = YawRotation.RotateVector(MyController->GetDirection());
		GetMovementComponent()->Velocity = Direction*AscendSpeed;
	}
}

void ASonyaWitchesCharacter::Descend()
{
	if ((MyController != NULL))
	{
		// find out which way is forward
		const FRotator Rotation = MyController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get up vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		GetMovementComponent()->Velocity = GetMovementComponent()->Velocity + Direction * DescendSpeed * -1.0;
	}
}

void ASonyaWitchesCharacter::OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	FVector Normal = Hit.ImpactNormal;
	Normal.Normalize();
	if (abs(Normal.Z) < 0.6)
	{
		UE_LOG(LogTemp, Warning, TEXT("Z fine"));
		FVector Direction = FVector(Normal.X, Normal.Y, 0.0f);
		Direction.Normalize();
		FVector Input = FVector(MyController->GetDirection().X, MyController->GetDirection().Y, 0.0f);
		Input.Normalize();
		const FRotator Rotation = MyController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		Input = YawRotation.RotateVector(Input);
		Input = -Input;

		float Angle = FVector::DotProduct(Direction, Input);
		UE_LOG(LogTemp, Warning, TEXT("Angle = %f"), Angle);

		if (true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Result fine"));
			if (CanJump())
			{
				IsPushing = true;
				WallDirection = Direction;
				SetActorTickEnabled(true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("WallSlide"));
				IsWallSliding = 2;
				WallDirection = Direction;
				SetActorTickEnabled(true);
				SetActorRotation(WallDirection.Rotation());
			}
		}
	}

	if (CanJump())
	{
		DoubleJumpsLeft = DoubleJumps;
		AirDashesLeft = AirDashes;
	}
}

void ASonyaWitchesCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
}

/** Handles crouch **/
void ASonyaWitchesCharacter::StartCrouch()
{
	if ((MyController != NULL))
	{	
		//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		/*Actions[CROUCH]->bIsActing = true;*/
		//Crouch();
		if (GetCharacterMovement()->IsFalling() && !IsWallSliding)
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
		}
		else
		{
			SetActorLocation(GetActorLocation() - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2));
		}
		SetActorScale3D(FVector(1.0f, 1.0f, 0.5f));
	}

/*
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	if (CurrentLevel == "ThirdPersonExampleMap")
	{
		UGameplayStatics::OpenLevel(TheWorld, "HubWorld");
	}
	else
	{
		UGameplayStatics::OpenLevel(TheWorld, "ThirdPersonExampleMap");
	}*/
}

/** Handles uncrouch **/
void ASonyaWitchesCharacter::EndCrouch()
{
	if ((MyController != NULL))
	{
		//UnCrouch();
		if (GetCharacterMovement()->IsFalling() && !IsWallSliding)
		{
			SetActorLocation(GetActorLocation() - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2));
		}
		else
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2));
		}
		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}

void ASonyaWitchesCharacter::TestLevelLoad()
{
	GameMode->GoToLevel(1, GetWorld());
}

/** Charge jump **/
void ASonyaWitchesCharacter::ChargeJump()
{
	if ((MyController != NULL))
	{
		if (CanJump())
		{
			JumpPower = 0.0f;
			IsChargingJump = true;
			SetActorTickEnabled(true);
		}
		else if (IsWallSliding)
		{
			// Stuff, maybe
			JumpPower = 0.0f;
			IsChargingJump = true;
			SetActorTickEnabled(true);
		}
		else if (DoubleJumps > 0)
		{
			JumpPower = 0.0f;
			IsChargingJump = true;
			SetActorTickEnabled(true);
		}
	}
}

void ASonyaWitchesCharacter::EmberDance()
{
	FVector NewVelocity = FVector(MyController->GetDirection().X, MyController->GetDirection().Y, 0.0f);

	if (NewVelocity.IsNearlyZero())
	{
		NewVelocity = FVector(0.0f, 0.0f, EmberDanceUpFactor);
		MovementLagTime = EmberDanceUpLag;
		VelocityLockTime = 0.0f;
		LockedVelocity = FVector(0.0f);
		SetActorTickEnabled(true);
	}
	else
	{
		NewVelocity.Normalize();
		MovementLagTime = 0.0f;
		VelocityLockTime = 0.0f;
		LockedVelocity = FVector(0.0f);
	}

	NewVelocity = NewVelocity * EmberDanceSpeed;

	// find out which way is forward
	const FRotator Rotation = MyController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get up vector
	NewVelocity = YawRotation.RotateVector(NewVelocity);
	GetMovementComponent()->Velocity = NewVelocity;
}

/** Charge jump **/
void ASonyaWitchesCharacter::ReleaseJump()
{
	if ((MyController != NULL) && (JumpPower > 0.01f))
	{
		float JumpSpeed;
		if (JumpPower < 0.5)
		{
			JumpSpeed = LowJumpSpeed;
		}
		else if (JumpPower < 1.0)
		{
			JumpSpeed = MidJumpSpeed;
		}
		else if (JumpPower >= 1.0)
		{
			JumpSpeed = HighJumpSpeed;
		}

		if (CanJump())
		{
			Jump();
			// find out which way is forward
			const FRotator Rotation = MyController->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get up vector
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			Direction = YawRotation.RotateVector(Direction);
			FVector AddedVelocity = Direction * JumpSpeed;
			GetMovementComponent()->Velocity = GetMovementComponent()->Velocity + AddedVelocity;
		}
		else if (IsWallSliding)
		{
			Jump();
			UE_LOG(LogTemp, Warning, TEXT("Wall Jump"));
			// find out which way is forward
			const FRotator Rotation = MyController->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);


			// get up vector
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			FVector Input = FVector(MyController->GetDirection().X, MyController->GetDirection().Y, 0.0f);
			Input.Normalize();
			Input = YawRotation.RotateVector(Input);
			Input = UKismetMathLibrary::GetReflectionVector(Input, WallDirection);
			FVector AddedVelocity = Direction * JumpSpeed + Input * (JumpSpeed * 0.5);
			GetMovementComponent()->Velocity = AddedVelocity;

			SetActorRotation(WallDirection.Rotation());
			IsWallSliding = 0;
			MovementLagTime = 0.1;
			SetActorTickEnabled(true);
		}
		else if (DoubleJumpsLeft > 0)
		{
			Jump();
			// find out which way is forward
			const FRotator Rotation = MyController->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get up vector
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			Direction += MyController->GetXYDirection() * GetCharacterMovement()->AirControl;
			Direction = YawRotation.RotateVector(Direction);
			FVector AddedVelocity = Direction * JumpSpeed;
			GetMovementComponent()->Velocity = AddedVelocity;

			DoubleJumpsLeft--;
			AirDashesLeft = AirDashes;
		}

		JumpPower = 0.0;
		IsChargingJump = false;
		//StopJumping();
	}
}

FVector ASonyaWitchesCharacter::SpringFactor(FVector CurrentVelocity, FVector AddedVelocity)
{

	AddedVelocity.Normalize();
	float factor = FVector::DotProduct(CurrentVelocity, AddedVelocity) * (-1.0);

	FVector ContinuedVelocity = CurrentVelocity;
	ContinuedVelocity.Normalize();

	UE_LOG(LogTemp, Warning, TEXT("Factor was %f"), factor);
	if (factor > 0)
	{
		return AddedVelocity * factor + (1-factor)* ContinuedVelocity + CurrentVelocity;
	}
	else
	{
		return CurrentVelocity;
	}
}

void ASonyaWitchesCharacter::FlameSpiral()
{
	if ((MyController != NULL) && (MovementLagTime < 0.01))
	{
		if (!CanJump())
		{
			GetMovementComponent()->Velocity += FVector(0.0f, 0.0f, FlameSpiralFall);
		}
	}
}

void ASonyaWitchesCharacter::SparkFantasia()
{
	if ((MyController != NULL) && (MovementLagTime < 0.01))
	{
		FVector NewVelocity = FVector(MyController->GetDirection().X, MyController->GetDirection().Y, 0.0f);

		if (NewVelocity.IsNearlyZero())
		{
			NewVelocity = FVector(0.0f);
		}
		else
		{
			if (CanJump())
			{
				Jump();
				SetActorLocation(GetActorLocation()+FVector(0.0f,0.0f,0.1f));
			}
			NewVelocity.Normalize();
		}

		NewVelocity = NewVelocity * SparkFantasiaSpeed;

		// find out which way is forward
		const FRotator Rotation = MyController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get up vector
		//FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		NewVelocity = YawRotation.RotateVector(NewVelocity);
		//FVector AddedVelocity = Direction * AscendSpeed;
		//GetMovementComponent()->Velocity = AddedVelocity;
		GetMovementComponent()->Velocity = NewVelocity;

		MovementLagTime = SparkFantasiaLag;
		VelocityLockTime = SparkFantasiaLock;
		LockedVelocity = NewVelocity;
		SetActorTickEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("Did Spark Fantasia"));
	}

}

void ASonyaWitchesCharacter::Die()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ASonyaWitchesCharacter::Zoom(float Value)
{
	if (Value != 0.0)
	{
		CameraZoomPercent += Value * CameraZoomSpeed;

		if (CameraZoomPercent > 1.0)
		{
			CameraZoomPercent = 1.0;
		}
		else if (CameraZoomPercent < 0.0)
		{
			CameraZoomPercent = 0.0;
		}

		CameraBoom->TargetArmLength = (CameraZoomPercent)*MinCameraDistance + (1-CameraZoomPercent)*MaxCameraDistance;
	}
}

void ASonyaWitchesCharacter::QuickDash()
{
	if (!CanJump() && AirDashesLeft > 0)
	{
		AirDashesLeft--;
	}
}

bool ASonyaWitchesCharacter::InputWasQuickDash()
{
	return true;
}

ASonyaPlayerController * ASonyaWitchesCharacter::GetMyController()
{
	return MyController;
}

void ASonyaWitchesCharacter::Sprint()
{
	if ((MyController != NULL))
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->MaxAcceleration = SprintSpeed * 2;
	}
}

void ASonyaWitchesCharacter::StopSprinting()
{
	if ((MyController != NULL))
	{
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
		GetCharacterMovement()->MaxAcceleration = JogSpeed * 2;
	}
}