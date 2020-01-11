// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBallInnerComponent.h"
#include "FireBallCoreComponent.h"
#include "FireBallShellComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TrackMovementComponent.h"
#include "Track.h"
#include "MagicCircle.h"
#include "FireBall.generated.h"

UCLASS()
class SONYAWITCHES_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBall(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveAlongSpline(float Alpha);

	// The Core Component
	UPROPERTY(EditAnywhere)
	class UFireBallCoreComponent* CoreComponent;

	// The Shell Component
	UPROPERTY(EditAnywhere)
	class UFireBallShellComponent* ShellComponent;

	// The main collision sphere
	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionComponent;

	// The Spline Component
	class USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	// Timeline things
	UPROPERTY(EditAnywhere)
		class UTrackMovementComponent *MyTrackMovementComponent;

		class AMagicCircle* OwningCircle;

		bool HasOwningCircle;

protected:
	// 0 = Normal, 1 = Homing, 2 = Periodic Homing
	UPROPERTY(EditAnywhere)
		int FlyType;
	// 0 = Normal, 1 = Shrapnel, 2 = Bounce
	UPROPERTY(EditAnywhere)
		int HitType;
public:	

	AMagicCircle* GetOwningCircle();
	void SetOwningCircle(AMagicCircle* NewCircle);

	bool GetHasOwningCircle();
	void SetHasOwningCircle(bool NewValue);

	UProjectileMovementComponent* GetProjectileMovementComponent();

	UFUNCTION()
	void OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
