// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "ButtonComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "ConstructorHelpers.h"

// Sets default values
AMenu::AMenu()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	USceneComponent* dummy = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(dummy);

	// Create and position a mesh component that will be our root
	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	ScreenMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ScreenMesh->SetupAttachment(this->RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ScreenMeshVisualAsset(TEXT("/Game/Meshes/Shape_Plane.Shape_Plane"));
	if (ScreenMeshVisualAsset.Succeeded())
	{
		ScreenMesh->SetStaticMesh(ScreenMeshVisualAsset.Object);
		ScreenMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ScreenMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
		ScreenMesh->SetWorldScale3D(FVector(5.0,10.0,1.0));
	}

	// Init the button component
	MyButtons[0] = CreateDefaultSubobject<UButtonComponent>(FName("Button0"));
	MyButtons[0]->GetRootComponent()->SetupAttachment(this->RootComponent);
	MyButtons[0]->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MyButtons[0]->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	MyButtons[1] = CreateDefaultSubobject<UButtonComponent>(FName("Button1"));
	MyButtons[1]->GetRootComponent()->SetupAttachment(this->RootComponent);
	MyButtons[1]->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MyButtons[1]->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	// Create a camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	MyButtons[0]->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	MyButtons[1]->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

	Camera->SetRelativeLocation(FVector(600.0f, 0.0f, 0.0f));

	Camera->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

}

// Called when the game starts or when spawned
void AMenu::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMenu::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMenu::TestSwitchingLevels);
}

void AMenu::CycleButtons()
{

}

void AMenu::GoToFileSelect()
{

}

void AMenu::ExitGame()
{

}

void AMenu::TestSwitchingLevels()
{
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	if (CurrentLevel == "ThirdPersonExampleMap")
	{
		UGameplayStatics::OpenLevel(TheWorld, "HubWorld");
	}
	else
	{
		UGameplayStatics::OpenLevel(TheWorld, "ThirdPersonExampleMap");
	}
	/*
	ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(GameMode->ContextObject, "HubWorld");
	if (pStreaming && pStreaming->IsLevelLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading HubWorld"));
		GameMode->UnloadLevel("HubWorld");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading HubWorld"));
		GameMode->LoadLevel("HubWorld");
	}

	pStreaming = UGameplayStatics::GetStreamingLevel(GameMode->ContextObject, "TitleScreen");
	if (pStreaming && pStreaming->IsLevelLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Unloading TitleScreen"));
		GameMode->UnloadLevel("TitleScreen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading TitleScreen"));
		GameMode->LoadLevel("TitleScreen");
	}
	
	
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("No world"));
		return;
	}

	ULevelStreaming* pStreaming = UGameplayStatics::GetStreamingLevel(GameMode->ContextObject, "TitleScreen");

	if (!pStreaming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not streaming"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Was streaming once"));

	if (pStreaming->ShouldBeLoaded())
	{
		pStreaming->SetShouldBeLoaded(true);
		pStreaming->SetShouldBeVisible(true);
		pStreaming->SetIsRequestingUnloadAndRemoval(false);

		pStreaming = UGameplayStatics::GetStreamingLevel(GameMode->ContextObject, "TitleScreen");

		pStreaming->SetShouldBeLoaded(false);
		pStreaming->SetShouldBeVisible(false);
		pStreaming->SetIsRequestingUnloadAndRemoval(true);
	}
	else
	{
		pStreaming->SetShouldBeLoaded(false);
		pStreaming->SetShouldBeVisible(false);
		pStreaming->SetIsRequestingUnloadAndRemoval(true);

		pStreaming = UGameplayStatics::GetStreamingLevel(GameMode->ContextObject, "TitleScreen");

		pStreaming->SetShouldBeLoaded(true);
		pStreaming->SetShouldBeVisible(true);
		pStreaming->SetIsRequestingUnloadAndRemoval(false);
	}
	*/
}


void AMenu::SetGameMode(class ASonyaWitchesGameMode* NewMode)
{
	this->GameMode = NewMode;
}