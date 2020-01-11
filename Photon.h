// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Photon.generated.h"

UCLASS()
class SONYAWITCHES_API APhoton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhoton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Bounce(const FHitResult& Hit);
	void BounceChase(const FHitResult& Hit);
	void AddRandomAngle(const FHitResult& Hit);
	void AddRandomSpeed();
	void LockAxes();

protected:
	AActor* TargetReference;

	FVector Velocity;

protected:
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere)
	USphereComponent* ChaseSphere;

protected:
	UPROPERTY(EditAnywhere)
	FVector InitialDirection;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BounceChaseAmount;

	UPROPERTY(EditAnywhere)
	float MinRandomSpeed;

	UPROPERTY(EditAnywhere)
	float MaxRandomSpeed;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BounceRandomVariance;

	UPROPERTY(EditAnywhere)
	bool bLockedXAxis;

	UPROPERTY(EditAnywhere)
	bool bLockedYAxis;

	UPROPERTY(EditAnywhere)
	bool bLockedZAxis;

	UPROPERTY(EditAnywhere)
	bool bRandomBounceSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
