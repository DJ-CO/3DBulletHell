// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TrackMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MagicCircle.generated.h"

UCLASS()
class SONYAWITCHES_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicCircle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AActor* TargetReference;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent * CircleMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent * ActivationSphere;

	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere)
	class UTrackMovementComponent* MyTrackMovementComponent;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent * MyProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	bool CanAim;

	UPROPERTY(EditAnywhere)
	float MaxTurnRate;

	UPROPERTY(EditAnywhere)
	float AimTime;

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots;

	float TimeUntilNextShot;

	UPROPERTY(EditAnywhere)
	float BulletSpeed;

	UPROPERTY(EditAnywhere)
	FVector AimOffset;

	UPROPERTY(EditAnywhere)
	bool UseSmartShot;

	UPROPERTY(EditAnywhere)
	bool UseHomingBullets;

	UPROPERTY(EditAnywhere)
	float HomingAcceleration;

	UPROPERTY(EditAnywhere)
	bool KeepAtDistance;

	UPROPERTY(EditAnywhere)
	float MinDistance;

protected:

	void AimAtTarget();

	void MoveAway();

	void Shoot(float DeltaTime);

	void QuadraticFormula(float A, float B, float C, float* PlusAnswer, float* MinusAnswer);

protected:
	UPROPERTY(EditAnywhere)
		bool FollowsTarget;

	// 0 = Single,
	UPROPERTY(EditAnywhere)
		int ShotType;

	UPROPERTY(EditAnywhere)
		int ShotRows;
	UPROPERTY(EditAnywhere)
		int ShotCols;
	UPROPERTY(EditAnywhere)
		float ShotSpacing;
	UPROPERTY(EditAnywhere)
		float ShotSpread;
	UPROPERTY(EditAnywhere)
		FVector BulletScale;
	UPROPERTY(EditAnywhere)
		bool ShotCurves;
protected:
	void SingleShot(FVector Direction, FVector Offset);
	void WallShot(FVector Up, FVector Right);

public:	

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
