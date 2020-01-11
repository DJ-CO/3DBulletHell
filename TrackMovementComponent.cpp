// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackMovementComponent.h"
#include "ConstructorHelpers.h"

// Sets default values for this component's properties
UTrackMovementComponent::UTrackMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// For timeline
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curves/BaseCurve"));
	check(Curve.Succeeded());

	FloatCurve = Curve.Object;

	CopyLocation = true;
	CopyRotation = true;
	// ...
}

void UTrackMovementComponent::Init(AActor* ActorToMove)
{
	MyActorToMove = ActorToMove;
}

// Called when the game starts
void UTrackMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MyTrack)
	{
		SplineComponent = MyTrack->GetSplineComponent();
	}

	LastLocation = FVector(0);

	LastRotation = FRotator(0);

	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	if (FloatCurve != NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("FloatCurve Not Null"));
		MyTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		MyActorToMove->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
		MyTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

	
		MyTimeline->SetPlayRate(PlayRate);

		if (StartOffset < 0)
		{
			MyTimeline->SetNewTime(0);
		}
		else if (StartOffset > 1)
		{
			MyTimeline->SetNewTime(1);
		}
		else
		{
			MyTimeline->SetNewTime(StartOffset);
		}

		MyTimeline->SetLooping(true);
		MyTimeline->SetTimelineLength(1.0f);
		MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		MyTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		MyTimeline->RegisterComponent();

		PlayTimeline();
	}
	// ...
	
}


// Called every frame
void UTrackMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyTimeline != NULL)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Timeline not Null"));
		MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	// ...
}


void UTrackMovementComponent::MoveAlongSpline(float Alpha)
{
	//UE_LOG(LogTemp, Warning, TEXT("Alpha = %f"), Alpha);
	//UE_LOG(LogTemp, Warning, TEXT("Moving"));
	float Distance = FMath::Lerp(0.0f, SplineComponent->GetSplineLength(), Alpha);
	//UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), Distance);
	FVector Location;
	FRotator Rotation;
	if (IsLocal)
	{
		FVector TempLocation;
		FRotator TempRotation;

		if (CopyLocation)
		{
			TempLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
			Location = (TempLocation - LastLocation) + MyActorToMove->GetActorLocation();
			LastLocation = TempLocation;
		}
		
		if (CopyRotation)
		{
			TempRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
			Rotation = (TempRotation - LastRotation) + MyActorToMove->GetActorRotation();
			LastRotation = TempRotation;
		}
	}
	else
	{
		if (CopyLocation)
		{
			Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		}
		
		if (CopyRotation)
		{
			Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		}
	}

	if (CopyLocation)
	{
		MyActorToMove->SetActorLocation(Location, false);
	}

	if (CopyRotation)
	{
		MyActorToMove->SetActorRotation(Rotation);
	}
	
}

void UTrackMovementComponent::TimelineCallback(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("In Callback"));
	if (MyTimeline && SplineComponent)
	{
		MoveAlongSpline(Value);
	}
}

void UTrackMovementComponent::TimelineFinishedCallback()
{

}

void UTrackMovementComponent::PlayTimeline()
{
	if (MyTimeline && SplineComponent)
	{
		if (StartOffset < 0)
		{
			MyTimeline->SetNewTime(0);
		}
		else if (StartOffset > 1)
		{
			MyTimeline->SetNewTime(1);
		}
		else
		{
			MyTimeline->SetNewTime(StartOffset);
		}

		MyTimeline->Play();
	}
}