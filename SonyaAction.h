//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "SonyaWitchesCharacter.h"
//#include "CoreMinimal.h"
//#include "SonyaAction.generated.h"
//
//enum ACTION
//{
//	JUMP,
//	CROUCH,
//	MOVE,
//	NUM_ACTIONS
//};
//
///**
// * 
// */
//UCLASS(config = Game)
//class SONYAWITCHES_API USonyaAction : UObject
//{
//public:
//	SonyaAction();
//	virtual ~SonyaAction();
//
//public:
//	class ASonyaWitchesCharacter *Sonya;
//
//	class ASonyaPlayerController* Controller;
//
//	bool bIsActing;
//
//protected:
//	float ActiveTime;
//
//public:
//	virtual void StartAction();
//	virtual void DoAction(float DeltaTime);
//};
//
//UCLASS(config = Game)
//class SONYAWITCHES_API USonyaJump : public USonyaAction
//{
//
//public:
//	virtual void StartAction() override;
//	virtual void DoAction(float DeltaTime) override;
//};
//
//UCLASS(config = Game)
//class SONYAWITCHES_API USonyaCrouch : public USonyaAction
//{
//
//public:
//	virtual void StartAction() override;
//	virtual void DoAction(float DeltaTime) override;
//};
