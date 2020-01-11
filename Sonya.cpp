// Fill out your copyright notice in the Description page of Project Settings.


#include "Sonya.h"

// Sets default values
ASonya::ASonya()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASonya::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASonya::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASonya::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

