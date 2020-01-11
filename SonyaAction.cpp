//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "SonyaAction.h"
//
//SonyaAction::SonyaAction()
//{
//	bIsActing = false;
//
//}
//
//SonyaAction::~SonyaAction()
//{
//}
//
//void SonyaAction::StartAction()
//{
//
//}
//
//void SonyaAction::DoAction(float DeltaTime)
//{
//
//}
//
//// JUMP
//void SonyaJump::StartAction()
//{
//	bIsActing = true;
//	ActiveTime = 0;
//}
//
//void SonyaJump::DoAction(float DeltaTime)
//{
//	if (ActiveTime < 0.1 && Controller->GetIsJumpPressed())
//	{
//		ActiveTime += DeltaTime;
//	}
//	else
//	{
//	}
//}
//
//void SonyaCrouch::StartAction()
//{
//	bIsActing = true;
//	Sonya->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 0.5f));
//}
//
//void SonyaCrouch::DoAction(float DeltaTime)
//{
//	if (Controller->GetIsCrouchPressed())
//	{
//
//	}
//	else
//	{
//		Sonya->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
//	}
//}