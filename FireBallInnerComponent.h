// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FireBallInnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API UFireBallInnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireBallInnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Is the ball a good ball
	UPROPERTY(EditAnywhere)
	bool bIsGood;

	// The mesh
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent * Mesh;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionSphere;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComponent;

	// The sphere for collision
	UPROPERTY(EditAnywhere)
	float Radius;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Attach to an actor
	void SetUpAttachment(USceneComponent* OwningRoot);

	class USceneComponent* GetRootComponent();
};
