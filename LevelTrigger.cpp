// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SonyaWitchesCharacter.h"

// Sets default values
ALevelTrigger::ALevelTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(50.0), true);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ALevelTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALevelTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	if (OtherActor && (OtherActor!=this) && OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Level"));
		UGameplayStatics::OpenLevel(GetWorld(), Level);
	}
}