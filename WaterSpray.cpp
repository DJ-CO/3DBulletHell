// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterSpray.h"
#include "SonyaWitchesCharacter.h"

// Sets default values
AWaterSpray::AWaterSpray()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DummyBox = CreateDefaultSubobject<UBoxComponent>("RootComponent");
	RootComponent = DummyBox;
	DummyBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	DummyBox->SetGenerateOverlapEvents(true);

	/*CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CollisionCapsule->SetupAttachment(RootComponent);
	CollisionCapsule->SetCapsuleHalfHeight(50.0);
	CollisionCapsule->SetCapsuleRadius(25.0);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionCapsule->SetGenerateOverlapEvents(true);*/
	//CollisionCapsule->OnComponentHit.AddDynamic(this, &AWaterSpray::OnCompHit);

	SprayTubeMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SprayTubeMesh");
	SprayTubeMesh->SetupAttachment(RootComponent);
	SprayTubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SprayTubeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SprayTubeMesh->SetGenerateOverlapEvents(true);

	SprayEndMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SprayEndMesh");
	SprayEndMesh->SetupAttachment(RootComponent);
	SprayEndMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SprayEndMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SprayEndMesh->SetGenerateOverlapEvents(true);

	//CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWaterSpray::OnOverlapBegin);
	SprayTubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AWaterSpray::OnOverlapBegin);
	SprayEndMesh->OnComponentBeginOverlap.AddDynamic(this, &AWaterSpray::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWaterSpray::BeginPlay()
{
	Super::BeginPlay();
	DummyBox->SetHiddenInGame(true);
	SprayTubeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	/*CollisionCapsule->SetRelativeLocation(FVector(50.f, 0.0f, 0.0f));
	CollisionCapsule->SetRelativeRotation(FRotator(90.f, 0.0f, 0.0f));*/
	SprayEndMesh->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	
}

// Called every frame
void AWaterSpray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterSpray::OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("SprayHitSomething"));
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter* Sonya = (ASonyaWitchesCharacter*)OtherActor;
		Sonya->Die();
	}
	else
	{
		float Size = (Hit.Location - (SprayTubeMesh->RelativeLocation + GetActorLocation())).Size();
		ScaleSpray(Size / 100.0f);
	}
}

void AWaterSpray::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Spray Overlapping Something"));
	if (OtherActor->IsA(ASonyaWitchesCharacter::StaticClass()))
	{
		ASonyaWitchesCharacter* Sonya = (ASonyaWitchesCharacter*)OtherActor;
		Sonya->Die();
	}
	else
	{
		float Size = (SweepResult.Location - (SprayTubeMesh->RelativeLocation + GetActorLocation())).Size();
		ScaleSpray(Size / 100.0f);
	}
}

void AWaterSpray::ScaleSpray(float Scale)
{
	if (Scale > 0)
	{
		SprayTubeMesh->SetRelativeScale3D(FVector(Scale, 1.0, 1.0));
		SprayEndMesh->SetRelativeLocation(FVector(Scale*100, 0.0f, 0.0f));
		//CollisionCapsule->SetCapsuleHalfHeight((Scale/2.0) * 100.0f + 15);
		//CollisionCapsule->SetRelativeLocation(FVector(((Scale / 2.0) * 100.0f + 15), 0.0f, 0.0f));
	}
	
}