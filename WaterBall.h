// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "WaterBall.generated.h"

UCLASS()
class SONYAWITCHES_API AWaterBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterBall();

	bool bShouldDieOnHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bShouldSplash;

	void Splash();

	/** Skeletal Mesh Comp, Set In BP Default Properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SkelMeshComponents)
	class USkeletalMeshComponent * BallMesh;


	/** Static Mesh Comp, Set In BP Default Properties */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
		//class UStaticMeshComponent * BallMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SkelMeshComponents)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	//UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Handles Hits **/
	UFUNCTION()
	void OnCompHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	bool GetShouldSplash();

	UFUNCTION(BlueprintCallable)
	void SetShouldDieOnHit(bool Value);

	class UProjectileMovementComponent* GetProjectileMovementComponent();
};
