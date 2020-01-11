// Fill out your copyright notice in the Description page of Project Settings.


#include "SprinklerShot.h"
#include "Sprinkler.h"
#include "Animation/AnimInstance.h"


// Sets default values
ASprinklerShot::ASprinklerShot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ShotMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ShotMesh");
	ShotMesh->SetGenerateOverlapEvents(true);
	RootComponent = ShotMesh;
	WaterSpray = NULL;
	Next = NULL;

	ShotStartDistance = 100.0f;
}

// Called when the game starts or when spawned
void ASprinklerShot::BeginPlay()
{
	Super::BeginPlay();


	if (ParentSprinkler)
	{

		if (ParentSprinkler->Head == NULL)
		{
			ParentSprinkler->Head = this;
		}
		else
		{
			ASprinklerShot* Current = ParentSprinkler->Head;

			while (Current->Next != NULL)
			{
				Current = Current->Next;
			}

			Current->Next = this;
		}

		FAttachmentTransformRules Rules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		this->AttachToComponent(ParentSprinkler->GetShaftMesh(), Rules);

		//SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	}
	
	//ShotMesh->GetAnimInstance()->;
}

// Called every frame
void ASprinklerShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASprinklerShot::Shoot(float SpeedScale)
{
	FActorSpawnParameters SpawnInfo;
	FVector Location = GetActorLocation() + GetActorForwardVector() * ShotStartDistance;
	FRotator Rotation = (FRotator)GetActorRotation();
	Rotation = FRotator(0.0f);
	//UE_LOG(LogTemp, Warning, TEXT("World Location is (%f , %f , %f)"), Location.X, Location.Y, Location.Z);
	//UE_LOG(LogTemp, Warning, TEXT("In shooter's \"Shoot()\""));

	if (WaterBallBPReference)
	{
		AWaterBall* WaterBall = GetWorld()->SpawnActor<AWaterBall>(WaterBallBPReference, Location, Rotation);
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Location is (%f , %f , %f)"), WaterBall->GetActorLocation().X, WaterBall->GetActorLocation().Y, WaterBall->GetActorLocation().Z);
		WaterBall->GetProjectileMovementComponent()->MaxSpeed = 0;
		WaterBall->GetProjectileMovementComponent()->Velocity = GetActorForwardVector() * 1000 * SpeedScale;
		WaterBall->GetProjectileMovementComponent()->bSimulationEnabled = true;
		//WaterBall->SetActorScale3D(FVector(1.0));
		//WaterBall->bShouldDieOnHit = true;
	}

	bIsShooting = true;
	bIsSpraying = false;
	bIsIdle = false;
}


void ASprinklerShot::Spray(float Scale)
{
	if (!WaterSpray)
	{
		if (WaterSprayBPReference)
		{
			FActorSpawnParameters SpawnInfo;
			FVector Location = GetActorForwardVector() * 70;
			FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
			WaterSpray = GetWorld()->SpawnActor<AWaterSpray>(WaterSprayBPReference, Location, Rotation);

			FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
			WaterSpray->AttachToActor(this, Rules);
			//WaterSpray->SetActorRelativeLocation(GetForwardVector() * (10 + WaterSpray->GetRootComponent());
		}
	}

	if (WaterSpray)
	{
		WaterSpray->ScaleSpray(Scale);
	}

	bIsShooting = false;
	bIsSpraying = true;
	bIsIdle = false;
}


bool ASprinklerShot::GetIsShooting()
{
	return bIsShooting;
}

void ASprinklerShot::SetIsShooting(bool Value)
{
	bIsShooting = Value;
}

bool ASprinklerShot::GetIsSpraying()
{
	return bIsSpraying;
}

void ASprinklerShot::SetIsSpraying(bool Value)
{
	bIsSpraying = Value;
}

bool ASprinklerShot::GetIsIdle()
{
	return bIsIdle;
}

void ASprinklerShot::SetIsIdle(bool Value)
{
	bIsIdle = Value;
}
