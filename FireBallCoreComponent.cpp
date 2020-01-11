// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBallCoreComponent.h"
#include "ConstructorHelpers.h"
#include "SonyaWitchesCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UFireBallCoreComponent::UFireBallCoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Radius = 10.0f;

	// Our root component will be a sphere that reacts to physics
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CoreCollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->InitSphereRadius(Radius);
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// Set up collision
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UFireBallCoreComponent::OnOverlapBegin);

	CollisionSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CollisionSphere->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	this->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	this->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	// Create and position a mesh component so we can see where our sphere is
	CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMesh"));
	CoreMesh->SetupAttachment(RootComponent);
	CoreMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Meshes/FireBallMesh/FireBallCoreMesh.FireBallCoreMesh"));
	if (SphereVisualAsset.Succeeded())
	{
		CoreMesh->SetStaticMesh(SphereVisualAsset.Object);
		CoreMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		CoreMesh->SetWorldScale3D(FVector(1.0f));
	}

	// ...
}


// Called when the game starts
void UFireBallCoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFireBallCoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

class USceneComponent* UFireBallCoreComponent::GetRootComponent()
{
	return RootComponent;
}


void UFireBallCoreComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherComp != OverlappedComp) && OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter *Sonya = (ASonyaWitchesCharacter*)OtherActor;
		Sonya->Die();
	}
}