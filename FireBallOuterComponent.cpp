// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBallOuterComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UFireBallOuterComponent::UFireBallOuterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Radius = 80.0f;

	// Our root component will be a sphere that reacts to physics
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = CollisionSphere;
	CollisionSphere->InitSphereRadius(Radius);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));

	// Create and position a mesh component so we can see where our sphere is
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		Mesh->SetStaticMesh(SphereVisualAsset.Object);
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -Radius));
		Mesh->SetWorldScale3D(FVector(50.0 / Radius));
	}

	// ...
}


// Called when the game starts
void UFireBallOuterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFireBallOuterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Attach to an actor
void UFireBallOuterComponent::SetUpAttachment(USceneComponent* OwningRoot)
{
	RootComponent->SetupAttachment(OwningRoot);
	//Mesh->SetupAttachment(RootComponent);
}

class USceneComponent* UFireBallOuterComponent::GetRootComponent()
{
	return RootComponent;
}