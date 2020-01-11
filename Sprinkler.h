// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sprinkler.generated.h"

UCLASS()
class SONYAWITCHES_API ASprinkler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprinkler();

protected:
	void SprayAll(float Size);
	void ShootAll(float SpeedScale);

protected:
	float TotalTime;
	float LastShotValue;

	float RotationPeriod;
	float ShotPeriod;

	UPROPERTY(EditAnywhere)
	bool bHasTopBase;

	UPROPERTY(EditAnywhere)
	bool bHasBottomBase;

public:
	class ASprinklerShot* Head;

	class ASprinklerShot* Current;

protected:
	class USphereComponent* RootSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
	class UStaticMeshComponent * ShaftMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
	class UStaticMeshComponent* TopBaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
	class UStaticMeshComponent* BottomBaseMesh;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* ShotCurve;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* RotationCurve;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UStaticMeshComponent* GetShaftMesh();

};
