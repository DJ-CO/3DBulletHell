// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBall.h"
#include "UObject/ConstructorHelpers.h"
#include "SonyaWitchesCharacter.h"
#include "Sprinkler.h"
#include "SprinklerShot.h"

// Sets default values
AWaterBall::AWaterBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = Sphere;
	//Sphere = CreateDefaultSubobject<USphereComponent>("RootComponent");
	//SetRootComponent(Sphere);
	//Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);

	BallMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BallMesh");
	RootComponent = BallMesh;
	BallMesh->SetRelativeLocation(FVector(0.0f));
	//BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BallMesh->SetCollisionProfileName(TEXT("Pawn"));
	BallMesh->OnComponentHit.AddDynamic(this, &AWaterBall::OnCompHit);
	BallMesh->SetGenerateOverlapEvents(true);
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AWaterBall::OnOverlapBegin);
	//BallMesh->OnComponentEndOverlap.AddDynamic(this, &AWaterBall::OnOverlapEnd);
	//RootComponent = BallMesh;
	//BallMesh->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>("OverlapSphere");
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Sphere->SetGenerateOverlapEvents(false);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AWaterBall::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AWaterBall::OnOverlapEnd);
	//Sphere->OnComponentHit.AddDynamic(this, &AWaterBall::OnCompHit);
	Sphere->SetupAttachment(RootComponent);
	//SetRootComponent(Sphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->UpdatedComponent = RootComponent;
	ProjectileMovementComponent->bSimulationEnabled = false;

	bShouldDieOnHit = false;
}

// Called when the game starts or when spawned
void AWaterBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (BallMesh)
	{
		//RootComponent = BallMesh;
		/*BallMesh->SetCollisionResponseToAllChannels(ECR_Block);
		BallMesh->SetCollisionProfileName(TEXT("PhysicsBody"));
		BallMesh->OnComponentHit.AddDynamic(this, &AWaterBall::OnCompHit);
		BallMesh->SetRelativeLocation(FVector(0.0f));*/
		//BallMesh->AttachTo(RootComponent);
	}
}

// Called every frame
void AWaterBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterBall::OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("WaterBall Colliding with something"));
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter * Sonya = (ASonyaWitchesCharacter*)OtherActor;
		Sonya->Die();
		if (bShouldDieOnHit)
		{
			Splash();
		}
	}
	else if (OtherActor != this)
	{
		if (bShouldDieOnHit)
		{
			Splash();
		}
	}
}

void AWaterBall::Splash()
{
	//FIXME
	UE_LOG(LogTemp, Warning, TEXT("WaterBall Splashing"));
	bShouldSplash = true;
	//this->Destroy();
}

class UProjectileMovementComponent* AWaterBall::GetProjectileMovementComponent()
{
	return this->ProjectileMovementComponent;
}

bool AWaterBall::GetShouldSplash()
{
	return bShouldSplash;
}

void AWaterBall::SetShouldDieOnHit(bool Value)
{
	bShouldDieOnHit = Value;

	if (bShouldDieOnHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trying to set collision"));
		BallMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AWaterBall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap Began"));
	UE_LOG(LogTemp, Warning, TEXT("WaterBall Overlapping with something"));
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter* Sonya = (ASonyaWitchesCharacter*)OtherActor;
		Sonya->Die();
		if (bShouldDieOnHit)
		{
			Splash();
		}
	}
	else if (OtherActor != this)
	{
		if (bShouldDieOnHit)
		{
			Splash();
		}
	}
}

void AWaterBall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap Ended"));
	if (OtherActor->IsA(ASprinklerShot::StaticClass()))
	{
		SetShouldDieOnHit(true);
	}
}
