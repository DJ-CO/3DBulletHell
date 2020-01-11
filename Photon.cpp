// Fill out your copyright notice in the Description page of Project Settings.


#include "Photon.h"
#include "SonyaWitchesCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APhoton::APhoton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("RootComponent");
	CollisionSphere->InitSphereRadius(50.0);
	CollisionSphere->OnComponentHit.AddDynamic(this, &APhoton::OnCompHit);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionSphere);

	ChaseSphere = CreateDefaultSubobject<USphereComponent>("ChaseSphere");
	ChaseSphere->InitSphereRadius(1000.0f);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ChaseSphere->OnComponentBeginOverlap.AddDynamic(this, &APhoton::OnOverlapBegin);
	ChaseSphere->OnComponentEndOverlap.AddDynamic(this, &APhoton::OnOverlapEnd);

	TargetReference = NULL;
	Speed = 100.0f;
	BounceChaseAmount = 0.0f;
	InitialDirection = FVector(1.0f,0.0f,0.0f);
	MinRandomSpeed = 50.0f;
	MaxRandomSpeed = 200.0f;
	BounceRandomVariance = 0.0f;
	bLockedXAxis = false;
	bLockedYAxis = false;
	bLockedZAxis = false;
	bRandomBounceSpeed = false;
}

// Called when the game starts or when spawned
void APhoton::BeginPlay()
{
	Super::BeginPlay();
	
	InitialDirection.Normalize();
	Velocity = InitialDirection * Speed;
	LockAxes();
}

// Called every frame
void APhoton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + Velocity * DeltaTime, true);
}

void APhoton::OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit"));
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter* Sonya = (ASonyaWitchesCharacter*)OtherActor;
		//Sonya->Die();
	}
	else if (this != OtherActor)
	{
		Bounce(Hit);
	}
}

void APhoton::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		TargetReference = OtherActor;
	}
}

void APhoton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		TargetReference = NULL;
	}
}

void APhoton::Bounce(const FHitResult& Hit)
{
	Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, Hit.Normal);
	
	if (TargetReference && BounceChaseAmount > 0.0f)
	{
		BounceChase(Hit);
	}

	if (BounceRandomVariance > 0.0f)
	{
		AddRandomAngle(Hit);
	}

	if (bRandomBounceSpeed)
	{
		AddRandomSpeed();
	}

	LockAxes();
}

void APhoton::BounceChase(const FHitResult& Hit)
{
	if (TargetReference)
	{
		FVector ChaseDirection = TargetReference->GetActorLocation() - GetActorLocation();
		ChaseDirection.Normalize();

		if (FVector::DotProduct(ChaseDirection, Hit.Normal) > 0.0f)
		{
			Velocity.Normalize();
			Velocity = (1.0 - BounceChaseAmount) * Velocity + BounceChaseAmount * ChaseDirection;
			Velocity *= Speed;
		}
	}
}

void APhoton::AddRandomAngle(const FHitResult& Hit)
{
	FVector NewDirection = FVector(FMath::RandRange(0, 100), FMath::RandRange(-100, 100), FMath::RandRange(-100, 100));
	FRotator Rotation = Velocity.Rotation();
	NewDirection.Normalize();
	NewDirection = Rotation.RotateVector(NewDirection);
	
	Velocity.Normalize();
	Velocity = (1.0 - BounceRandomVariance) * Velocity + BounceRandomVariance * NewDirection;
	Velocity *= Speed;
}

void APhoton::AddRandomSpeed()
{
	float RandomAmount = FMath::RandRange(0, 1000) / 1000.0f;
	float RandomSpeed = (1.0 - RandomAmount) * MinRandomSpeed + (RandomAmount)* MaxRandomSpeed;
	Velocity.Normalize();
	Velocity *= RandomSpeed;
}

void APhoton::LockAxes()
{
	FVector RelativeVelocity = Velocity;
	FRotator Rotation = FRotator(0.0f) - GetActorRotation();
	RelativeVelocity = Rotation.RotateVector(RelativeVelocity);

	if (bLockedXAxis)
	{
		RelativeVelocity.X = 0;
	}

	if (bLockedYAxis)
	{
		RelativeVelocity.Y = 0;
	}

	if (bLockedZAxis)
	{
		RelativeVelocity.Z = 0;
	}

	Rotation = GetActorRotation();
	RelativeVelocity = Rotation.RotateVector(RelativeVelocity);

	if (RelativeVelocity.Size() > 0.0f)
	{
		RelativeVelocity.Normalize();
	}
	
	Velocity = RelativeVelocity * Velocity.Size();
}