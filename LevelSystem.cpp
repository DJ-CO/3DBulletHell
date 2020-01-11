// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSystem.h"
#include "SonyaWitchesCharacter.h"

// Sets default values
ALevelSystem::ALevelSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	EndVolume = CreateDefaultSubobject<UBoxComponent>("EndVolume");
	EndVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelSystem::OnOverlapBegin);
	EndVolume->SetCollisionProfileName(TEXT("WorldDynamic"));
	EndVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(EndVolume);

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>("EndMesh");
	EndMesh->SetupAttachment(RootComponent);

	KeyVolume = CreateDefaultSubobject<USphereComponent>("KeyVolume");
	KeyVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelSystem::OnOverlapBegin);
	KeyVolume->SetCollisionProfileName(TEXT("WorldDynamic"));
	KeyVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	KeyVolume->SetupAttachment(RootComponent);

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>("KeyMesh");
	KeyMesh->SetupAttachment(KeyVolume);

	// Initialize Variables
	KeyIsCollected = false;
}

// Called when the game starts or when spawned
void ALevelSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSystem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != this) && (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass())))
	{
		if (OverlappedComp == this->EndVolume)
		{
			if (KeyIsCollected)
			{
				OnLevelEnd();
			}
		}

		if (OverlappedComp == this->KeyVolume)
		{
			OnKeyCollected();
		}
	}
}

void ALevelSystem::OnKeyCollected()
{
	KeyIsCollected = true;
	KeyMesh->DestroyComponent();
	KeyVolume->DestroyComponent();

}

void ALevelSystem::OnLevelEnd()
{
	UGameplayStatics::OpenLevel(GetWorld(), "HubWorld");
}