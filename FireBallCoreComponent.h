// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FireBallCoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API UFireBallCoreComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireBallCoreComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Is the ball a good ball
	UPROPERTY(EditAnywhere)
		bool bIsGood;

	// The mesh
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CoreMesh;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionSphere;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
		float Radius;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
		class USceneComponent* RootComponent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class USceneComponent* GetRootComponent ();
};
