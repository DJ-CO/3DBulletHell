// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FireBallShellComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API UFireBallShellComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireBallShellComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Is the ball a good ball
	UPROPERTY(EditAnywhere)
		bool bIsGood;

	// The mesh
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ShellMesh;

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

	class USceneComponent* GetRootComponent();
};
