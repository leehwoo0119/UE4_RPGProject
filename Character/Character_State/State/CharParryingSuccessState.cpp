// Fill out your copyright notice in the Description page of Project Settings.


#include "CharParryingSuccessState.h"
#include "CharStatusComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"

UCharParryingSuccessState::UCharParryingSuccessState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharParryingSuccessState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharParryingSuccessState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::ParryingSuccess);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharUltimatePoint(20.0f);
}

void UCharParryingSuccessState::Execute()
{
	Super::Execute();
	mainChar->GetAnimInstance()->AnimMontagePlay(
		mainChar->GetAnimInstance()->parryingSuccessMontage,
		1.0f,
		FName("ParryingSuccessStart"));
}

void UCharParryingSuccessState::Exit()
{
	Super::Exit();
}