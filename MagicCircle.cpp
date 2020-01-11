// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicCircle.h"
#include "SonyaWitchesCharacter.h"
#include "FireBall.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetSphereRadius(100.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("Pawn"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	RootComponent = CollisionSphere;
	SetRootComponent(CollisionSphere);
	ActivationSphere = CreateDefaultSubobject<USphereComponent>("ActivationSphere");
	ActivationSphere->SetupAttachment(RootComponent);
	ActivationSphere->SetRelativeLocation(FVector(0.0f));
	ActivationSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ActivationSphere->SetGenerateOverlapEvents(true);
	CircleMesh = CreateDefaultSubobject<UStaticMeshComponent>("CircleMesh");
	CircleMesh->SetupAttachment(RootComponent);
	CircleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MyTrackMovementComponent = CreateDefaultSubobject<UTrackMovementComponent>("TrackMovementComponent");
	MyTrackMovementComponent->Init(this);
	MyProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	MyProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	MyProjectileMovementComponent->InitialSpeed = 0.0f;
	MyProjectileMovementComponent->MaxSpeed = 500.0f;
	MyProjectileMovementComponent->bRotationFollowsVelocity = false;
	MyProjectileMovementComponent->bShouldBounce = false;
	MyProjectileMovementComponent->Bounciness = 0.3f;
	MyProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	MyProjectileMovementComponent->bSimulationEnabled = false;
	MyProjectileMovementComponent->bIsHomingProjectile = true;
	MyProjectileMovementComponent->HomingAccelerationMagnitude = 600.0f;

	ActivationSphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicCircle::OnOverlapBegin);
	ActivationSphere->OnComponentEndOverlap.AddDynamic(this, &AMagicCircle::OnOverlapEnd);
	
	TargetReference = NULL;
	CanAim = false;
	UseSmartShot = false;
	MaxTurnRate = 10.0f;
	AimTime = 0.5f;
	TimeBetweenShots = 1.0;
	TimeUntilNextShot = TimeBetweenShots;
	BulletSpeed = 2500.f;
	AimOffset = FVector(0.0f,0.0f,0.0f);
	ShotType = 0;
	BulletScale = FVector(1.0f);
	ShotRows = 1;
	ShotCols = 1;
	ShotSpacing = 50.f;
	ShotSpread = 1.0f;
	ShotCurves = false;
	KeepAtDistance = true;
	MinDistance = 2000.0f;
}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetReference)
	{
		if (CanAim)
		{
			AimAtTarget();
		}

		if (KeepAtDistance)
		{
			if ((TargetReference->GetActorLocation() - GetActorLocation()).Size() < MinDistance)
			{
				MoveAway();
			}
		}
		
		Shoot(DeltaTime);
	}
	else
	{
		SetActorTickEnabled(false);
	}
	
}

void AMagicCircle::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Magic circle overlap began"));
	if (OtherActor && (OtherComp != OverlappedComp) && OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		if (OverlappedComp == ActivationSphere)
		{
			TargetReference = OtherActor;
			SetActorTickEnabled(true);

			if (FollowsTarget)
			{
				MyProjectileMovementComponent->bSimulationEnabled = true;
				MyProjectileMovementComponent->bIsHomingProjectile = true;
				MyProjectileMovementComponent->HomingTargetComponent = OtherComp;
			}
		}
	}

}

void AMagicCircle::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Magic circle overlap ended"));
	if (OtherActor && (OtherComp != OverlappedComp) && (OverlappedComp == ActivationSphere) && OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		TargetReference = NULL;
		SetActorTickEnabled(false);

		if (FollowsTarget)
		{
			MyProjectileMovementComponent->bSimulationEnabled = false;
			MyProjectileMovementComponent->bIsHomingProjectile = true;
			MyProjectileMovementComponent->HomingTargetComponent = NULL;
			MyProjectileMovementComponent->Velocity = FVector(0.0f);
		}
	}
	if (OtherActor && (OtherComp != OverlappedComp) && (OverlappedComp == ActivationSphere) && OtherActor->IsA(AFireBall::StaticClass()))
	{
		AFireBall* FireBall = (AFireBall*)OtherActor;

		if (FireBall->GetHasOwningCircle() && FireBall->GetOwningCircle() == this)
		{
			FireBall->Destroy();
		}
	}
}

void AMagicCircle::AimAtTarget()
{
	if (TargetReference)
	{
		if (UseSmartShot && TargetReference->GetVelocity().Size() < BulletSpeed && TargetReference->GetVelocity().Size() > 0.0f)
		{
			FVector Displacement = TargetReference->GetActorLocation() - GetActorLocation();
			float Distance = Displacement.Size();
			float CosTheta = FVector::DotProduct(TargetReference->GetVelocity(), -Displacement) / (Distance * TargetReference->GetVelocity().Size());
			float Time;

			float A = BulletSpeed * BulletSpeed - TargetReference->GetVelocity().Size() * TargetReference->GetVelocity().Size();
			float B = 2 * TargetReference->GetVelocity().Size() * Distance * CosTheta;
			float C = -Distance * Distance;

			float TimeA;
			float TimeB;
			QuadraticFormula(A, B, C, &TimeA, &TimeB);

			if (TimeA > 0)
			{
				Time = TimeA;
			}
			else if (TimeB > 0)
			{
				Time = TimeB;
			}
			else
			{
				Time = 0;
			}

			FVector AimLocation = TargetReference->GetVelocity() * Time + TargetReference->GetActorLocation();
			this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), AimLocation + AimOffset));
		}
		else
		{
			this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TargetReference->GetActorLocation() + AimOffset));
		}
	}
}

void AMagicCircle::MoveAway()
{
	FVector NewLocation = GetActorLocation() - TargetReference->GetActorLocation();
	NewLocation.Normalize();
	NewLocation = NewLocation * MinDistance + TargetReference->GetActorLocation();
	SetActorLocation(NewLocation, true);
}

void AMagicCircle::Shoot(float DeltaTime)
{
	TimeUntilNextShot -= DeltaTime;

	if (TimeUntilNextShot < 0.0f)
	{
		switch (ShotType)
		{
		case 0:
			SingleShot(this->GetActorForwardVector(), FVector (0.0f));
			break;
		case 1:
			WallShot(this->GetActorUpVector(), this->GetActorRightVector());
			break;
		default:
			break;
		}
		
		TimeUntilNextShot = TimeBetweenShots;
	}
}

void AMagicCircle::QuadraticFormula(float A, float B, float C, float * PlusAnswer, float * MinusAnswer)
{
	*MinusAnswer = (-B - sqrtf(B * B - 4 * A * C)) / (2 * A);
	*PlusAnswer = (-B + sqrtf(B * B - 4 * A * C)) / (2 * A);
}

void AMagicCircle::SingleShot(FVector Direction, FVector Offset)
{
	FActorSpawnParameters SpawnInfo;
	AFireBall* FireBall = this->GetWorld()->SpawnActor<AFireBall>(this->GetActorLocation() + Offset, this->GetActorRotation(), SpawnInfo);
	FireBall->GetProjectileMovementComponent()->bSimulationEnabled = true;
	FireBall->GetProjectileMovementComponent()->MaxSpeed = 0;
	FireBall->GetProjectileMovementComponent()->Velocity = Direction * BulletSpeed;
	FireBall->GetProjectileMovementComponent()->bIsHomingProjectile = UseHomingBullets;
	FireBall->GetProjectileMovementComponent()->HomingAccelerationMagnitude = HomingAcceleration;
	FireBall->GetProjectileMovementComponent()->HomingTargetComponent = TargetReference->GetRootComponent();

	FireBall->SetHasOwningCircle(true);
	FireBall->SetOwningCircle(this);
	FireBall->SetActorScale3D(BulletScale);

}

void AMagicCircle::WallShot(FVector Up, FVector Right)
{
	FVector Direction = this->GetActorForwardVector();

	if (ShotRows == 0)
	{
		ShotRows = 1;
	}

	if (ShotCols == 0)
	{
		ShotCols = 1;
	}

	for (float i = -ShotRows / 2.0; i < (ShotRows / 2.0) - 0.5; i = i + 1)
	{
		for (float j = -ShotCols / 2.0; j < (ShotCols / 2.0) - 0.5; j = j + 1)
		{
			if (ShotCurves)
			{
				SingleShot(Direction + i * (ShotSpread / ShotRows) * Up + j * (ShotSpread / ShotCols) *Right, FVector(0.0f));
			}
			else
			{
				SingleShot(Direction, i * ShotSpacing * Up + j * ShotSpacing * Right);
			}
			
		}
	}
}