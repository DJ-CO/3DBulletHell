// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LevelSystem.generated.h"

UCLASS()
class SONYAWITCHES_API ALevelSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
protected:
	// Volume that triggers level end
	UPROPERTY(EditAnywhere)
	class UBoxComponent * EndVolume;

	// Mesh representing the end volume
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* EndMesh;

	// Volume that represents the key or pearl
	UPROPERTY(EditAnywhere)
	class USphereComponent* KeyVolume;

	// Mesh representing the key or pearl
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* KeyMesh;

	// Variables
protected:
	bool KeyIsCollected;

	// Functions
protected:
	void OnKeyCollected();
	void OnLevelEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handles Overlap events
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
