// Fill out your copyright notice in the Description page of Project Settings.


#include "SprinklerShotComponent.h"
#include "WaterBall.h"

// Sets default values for this component's properties
USprinklerShotComponent::USprinklerShotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	ShotMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ShotMesh");
	//RootComponent = BallMesh;
	ShotMesh->SetRelativeLocation(FVector(0.0f));
	PrimaryComponentTick.bCanEverTick = false;
	WaterSpray = NULL;
	Next = NULL;
	// ...
}


// Called when the game starts
void USprinklerShotComponent::BeginPlay()
{
	Super::BeginPlay();

	
	if (ParentSprinkler)
	{
		if (ParentSprinkler->Head == NULL)
		{
			//ParentSprinkler->Head = this;
		}
		else
		{
			//USprinklerShotComponent* Current = ParentSprinkler->Head;


			/*while (Current->Next != NULL)
			{
				Current = Current->Next;
			}*/

			//Current->Next = this;
		}

		SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	}
	
	

	// ...
	
}


// Called every frame
void USprinklerShotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USprinklerShotComponent::Shoot(float SpeedScale)
{
	FActorSpawnParameters SpawnInfo;
	FVector Location = GetComponentTransform().GetLocation() + GetForwardVector() * 50;
	FRotator Rotation = (FRotator)GetComponentTransform().GetRotation();
	Rotation = FRotator(0.0f);
	//UE_LOG(LogTemp, Warning, TEXT("World Location is (%f , %f , %f)"), Location.X, Location.Y, Location.Z);
	//UE_LOG(LogTemp, Warning, TEXT("In shooter's \"Shoot()\""));

	if (WaterBallBPReference)
	{
		AWaterBall* WaterBall = GetWorld()->SpawnActor<AWaterBall>(WaterBallBPReference, Location, Rotation);
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Location is (%f , %f , %f)"), WaterBall->GetActorLocation().X, WaterBall->GetActorLocation().Y, WaterBall->GetActorLocation().Z);
		WaterBall->GetProjectileMovementComponent()->MaxSpeed = 0;
		WaterBall->GetProjectileMovementComponent()->Velocity = GetForwardVector() * 1000 * SpeedScale;
		WaterBall->GetProjectileMovementComponent()->bSimulationEnabled = true;
		//WaterBall->SetActorScale3D(FVector(1.0));
		WaterBall->bShouldDieOnHit = true;
	}
}

void USprinklerShotComponent::Spray(float Scale)
{
	if (!WaterSpray)
	{
		if (WaterSprayBPReference)
		{
			FActorSpawnParameters SpawnInfo;
			FVector Location = GetForwardVector() * 10;
			FRotator Rotation = FRotator(-90.0f, 0.0f, 0.0f);
			WaterSpray = GetWorld()->SpawnActor<AWaterSpray>(WaterSprayBPReference, Location, Rotation);

			FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
			WaterSpray->AttachToComponent(this,Rules);
			//WaterSpray->SetActorRelativeLocation(GetForwardVector() * (10 + WaterSpray->GetRootComponent());
		}
	}
	
	if (WaterSpray)
	{
		WaterSpray->SetActorScale3D(FVector(1.0f, 1.0f, Scale));
	}
}