// Fill out your copyright notice in the Description page of Project Settings.


#include "CharCrouchState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "CharStatusComponent.h"

UCharCrouchState::UCharCrouchState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharCrouchState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharCrouchState::Enter()
{
	Super::Enter();

	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Crouch);
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Crouch);
}

void UCharCrouchState::Execute()
{
	Super::Execute();

	mainChar->Crouch();
}

void UCharCrouchState::Exit()
{
	Super::Exit();
}
