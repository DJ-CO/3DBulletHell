// Fill out your copyright notice in the Description page of Project Settings.


#include "Track.h"

// Sets default values
ATrack::ATrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent"));
	SplineComponent->SetupAttachment(this->RootComponent);
	SplineComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SplineComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void ATrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USplineComponent* ATrack::GetSplineComponent()
{
	return SplineComponent;
}
