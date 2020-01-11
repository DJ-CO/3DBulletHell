// Fill out your copyright notice in the Description page of Project Settings.

#include "Sprinkler.h"
#include "SprinklerShot.h"
#include "WaterBall.h"

// Sets default values
ASprinkler::ASprinkler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSphere = CreateDefaultSubobject<USphereComponent>("RootComponent");
	RootComponent = RootSphere;

	ShaftMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShaftMesh");
	ShaftMesh->SetupAttachment(RootComponent);

	TopBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("TopBaseMesh");
	TopBaseMesh->SetupAttachment(RootComponent);

	BottomBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BottomBaseMesh");
	BottomBaseMesh->SetupAttachment(RootComponent);

	bHasBottomBase = true;
	bHasTopBase = true;

	TotalTime = 0;
	ShotCurve = NULL;
	RotationCurve = NULL;
	LastShotValue = 0.0;
}

// Called when the game starts or when spawned
void ASprinkler::BeginPlay()
{
	Super::BeginPlay();

	RootSphere->SetHiddenInGame(true);

	if (ShotCurve)
	{
		ShotPeriod = ShotCurve->GetFloatValue(-1);
		if (ShotPeriod <= 0)
		{
			ShotPeriod = 1;
		}
	}

	if (RotationCurve)
	{
		RotationPeriod = RotationCurve->GetFloatValue(-1);
		if (RotationPeriod <= 0)
		{
			RotationPeriod = 1;
		}
	}

	if (bHasTopBase)
	{
		//TopBaseMesh->SetRelativeLocation(FVector(0.0f,0.0f,Height));
	}
	else
	{
		TopBaseMesh->SetHiddenInGame(true);
	}

	if (bHasBottomBase)
	{
		//BottomBaseMesh->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	}
	else
	{
		BottomBaseMesh->SetHiddenInGame(true);
	}
}

// Called every frame
void ASprinkler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	TotalTime += DeltaTime;

	if (TotalTime > 100.0)
	{
		TotalTime = TotalTime - floor(TotalTime / 100) * 100;
	}

	if (ShotCurve != NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Shot Curve not null"));
		float CurrentTime = TotalTime - floor(TotalTime / ShotPeriod) * ShotPeriod;
		float CurrentValue = ShotCurve->GetFloatValue(CurrentTime);
		if (CurrentValue > 0.98 && LastShotValue < 0.01)
		{
			ShootAll(CurrentValue);
		}
		else if (CurrentValue < 0.0)
		{
			SprayAll(-CurrentValue);
		}
		LastShotValue = CurrentValue;
	}

	if (RotationCurve != NULL)
	{
		float CurrentTime = TotalTime - floor(TotalTime / RotationPeriod) * RotationPeriod;
		float CurrentValue = RotationCurve->GetFloatValue(CurrentTime);
		ShaftMesh->SetRelativeRotation(FRotator(ShaftMesh->RelativeRotation.Pitch, CurrentValue, ShaftMesh->RelativeRotation.Roll));
		//SetActorRotation(FRotator(GetActorRotation().Pitch, CurrentValue, GetActorRotation().Roll));
	}
	
}

void ASprinkler::ShootAll(float SpeedScale)
{
	//UE_LOG(LogTemp, Warning, TEXT("Shooting"));
	Current = Head;
	while (Current != NULL)
	{
		Current->Shoot(SpeedScale);
		Current = Current->Next;
	}
}

void ASprinkler::SprayAll(float Scale)
{
	//UE_LOG(LogTemp, Warning, TEXT("Spraying"));
	Current = Head;
	while (Current != NULL)
	{
		Current->Spray(Scale);
		Current = Current->Next;
	}
}

class UStaticMeshComponent* ASprinkler::GetShaftMesh()
{
	return this->ShaftMesh;
}
