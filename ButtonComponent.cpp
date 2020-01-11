// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonComponent.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UButtonComponent::UButtonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Create and position a mesh component that will be our root
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	RootComponent = ButtonMesh;
	ButtonMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ButtonVisualAsset(TEXT("/Game/Meshes/Shape_Plane.Shape_Plane"));
	if (ButtonVisualAsset.Succeeded())
	{
		ButtonMesh->SetStaticMesh(ButtonVisualAsset.Object);
		ButtonMesh->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));
		ButtonMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
		ButtonMesh->SetWorldScale3D(FVector(1.0));
	}

	// Create the text component
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ButtonText"));
	TextComponent->SetText("Text");
	TextComponent->SetupAttachment(RootComponent);
	TextComponent->SetTextRenderColor(FColor(0.0f, 0.0f, 0.0f));
	TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
	TextComponent->SetRelativeRotation(FRotator(90.f, 0.0f, 0.0f));

	

	// ...
}


// Called when the game starts
void UButtonComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UButtonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UButtonComponent::OnSelected()
{
	RelativeLocation = RelativeLocation + FVector(20.0f, 0.0f, 0.0f);
	TextComponent->SetTextRenderColor(FColor(1.0f, 1.0f, 0.0f));
}

void UButtonComponent::OnDeselected()
{
	RelativeLocation = RelativeLocation + FVector(-20.0f, 0.0f, 0.0f);
	TextComponent->SetTextRenderColor(FColor(0.0f, 0.0f, 0.0f));
}

void UButtonComponent::OnPressed()
{

}

class USceneComponent* UButtonComponent::GetRootComponent()
{
	return RootComponent;
}

void UButtonComponent::ToggleSelected()
{
	if (bIsSelected)
	{
		bIsSelected = false;
		OnDeselected();
	}
	else
	{
		bIsSelected = true;
		OnSelected();
	}
}