// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBallShellComponent.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UFireBallShellComponent::UFireBallShellComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Radius = 30.0f;

	// Our root component will be a sphere that reacts to physics
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShellCollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->InitSphereRadius(Radius);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	CollisionSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CollisionSphere->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	this->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	this->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	// Create and position a mesh component so we can see where our sphere is
	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShellMesh"));
	ShellMesh->SetupAttachment(RootComponent);
	ShellMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Meshes/FireBallMesh/FireBallShellMesh.FireBallShellMesh"));
	if (SphereVisualAsset.Succeeded())
	{
		ShellMesh->SetStaticMesh(SphereVisualAsset.Object);
		ShellMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ShellMesh->SetWorldScale3D(FVector(1.5f));
	}

	// ...
}


// Called when the game starts
void UFireBallShellComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFireBallShellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

class USceneComponent* UFireBallShellComponent::GetRootComponent()
{
	return RootComponent;
}
