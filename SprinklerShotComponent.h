// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Sprinkler.h"
#include "WaterBall.h"
#include "WaterSpray.h"
#include "Components/SkeletalMeshComponent.h"
#include "SprinklerShotComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API USprinklerShotComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USprinklerShotComponent();

	void Shoot(float SpeedScale);
	void Spray(float Scale);

	UPROPERTY(EditAnywhere)
	class ASprinkler* ParentSprinkler;

	class USprinklerShotComponent* Next;

protected:
	// Called when the game starts
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
