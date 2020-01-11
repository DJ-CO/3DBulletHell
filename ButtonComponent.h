// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "ButtonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SONYAWITCHES_API UButtonComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UButtonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UStaticMeshComponent* ButtonMesh;

	bool bIsSelected;

	void OnSelected();

	void OnDeselected();

	void OnPressed();

	// Usually the mesh
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComponent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Usually the mesh
	UPROPERTY(EditAnywhere)
	class UTextRenderComponent* TextComponent;

	class USceneComponent* GetRootComponent();

	void ToggleSelected();
};
