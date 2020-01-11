// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Track.h"
#include "TrackMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API UTrackMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrackMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	class AActor * MyActorToMove;

	// The Spline Component
	class USplineComponent* SplineComponent;

	// The The Track
	UPROPERTY(EditAnywhere)
	class ATrack* MyTrack;

	// Timeline things
	UPROPERTY(EditAnywhere)
		class UTimelineComponent* MyTimeline;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* FloatCurve;

	UPROPERTY(EditAnywhere)
		float PlayRate;

	UPROPERTY(EditAnywhere)
		float StartOffset;

	UFUNCTION()
		void TimelineCallback(float Value);

	UFUNCTION()
		void TimelineFinishedCallback();

	UPROPERTY(EditAnywhere)
		bool IsLocal;

	UPROPERTY(EditAnywhere)
		bool CopyLocation;

	UPROPERTY(EditAnywhere)
		bool CopyRotation;

	void PlayTimeline();

	void MoveAlongSpline(float Alpha);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	FVector LastLocation;

	FRotator LastRotation;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(AActor* ActorToMove);	
};
