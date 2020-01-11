// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include "FireBallInnerComponent.h"
#include "FireBallCoreComponent.h"
#include "FireBallShellComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
AFireBall::AFireBall(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init the collision component
	CollisionComponent = OI.CreateDefaultSubobject<USphereComponent>(this, FName("RootComponent"));
	CollisionComponent->SetSphereRadius(10.0f);
	SetRootComponent(CollisionComponent);
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	CollisionComponent->OnComponentHit.AddDynamic(this, &AFireBall::OnCompHit);

	// Init the core component
	CoreComponent = OI.CreateDefaultSubobject<UFireBallCoreComponent>(this, FName("CoreComponent"));
	CoreComponent->GetRootComponent()->SetupAttachment(this->RootComponent);
	CoreComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CoreComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	// Init the shell component
	ShellComponent = OI.CreateDefaultSubobject<UFireBallShellComponent>(this, FName("ShellComponent"));
	ShellComponent->GetRootComponent()->SetupAttachment(this->RootComponent);
	ShellComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ShellComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	


	MyTrackMovementComponent = OI.CreateDefaultSubobject<UTrackMovementComponent>(this, FName("TrackMovementComponent"));
	MyTrackMovementComponent->Init(this);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bSimulationEnabled = false;

	OwningCircle = NULL;
	HasOwningCircle = false;
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	
	CoreComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMagicCircle* AFireBall::GetOwningCircle()
{
	return OwningCircle;
}

void AFireBall::SetOwningCircle(AMagicCircle* NewCircle)
{
	OwningCircle = NewCircle;
}

bool AFireBall::GetHasOwningCircle()
{
	return HasOwningCircle;
}

void AFireBall::SetHasOwningCircle(bool NewValue)
{
	HasOwningCircle = NewValue;
}

UProjectileMovementComponent* AFireBall::GetProjectileMovementComponent()
{
	return ProjectileMovementComponent;
}

void AFireBall::OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		this->Destroy();
	}
}