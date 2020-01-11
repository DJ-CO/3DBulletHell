// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "WaterBall.h"
#include "WaterSpray.h"
#include "SprinklerShot.generated.h"

UCLASS()
class SONYAWITCHES_API ASprinklerShot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprinklerShot();

	void Shoot(float SpeedScale);
	void Spray(float Scale);

	UPROPERTY(EditAnywhere)
	class ASprinkler* ParentSprinkler;

	class ASprinklerShot* Next;

protected:
	bool bIsShooting;

	bool bIsSpraying;

	bool bIsIdle;

	UPROPERTY(EditDefaultsOnly)
	float ShotStartDistance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Skeletal Mesh Comp, Set In BP Default Properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SkelMeshComponents)
	class USkeletalMeshComponent* ShotMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWaterBall> WaterBallBPReference;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWaterSpray> WaterSprayBPReference;

	class AWaterSpray* WaterSpray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsShooting();

	UFUNCTION(BlueprintCallable)
	void SetIsShooting(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsSpraying();

	UFUNCTION(BlueprintCallable)
	void SetIsSpraying(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsIdle();

	UFUNCTION(BlueprintCallable)
	void SetIsIdle(bool Value);

};
