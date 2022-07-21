// Fill out your copyright notice in the Description page of Project Settings.


#include "CharParryingState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharStatusComponent.h"

UCharParryingState::UCharParryingState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharParryingState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharParryingState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::CanParrying);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
	mainChar->GetCharStatusComponent()->SetMoveStop(true);
}


void UCharParryingState::Execute()
{
	Super::Execute();

	if (mainChar->GetVelocity().Size() == 0.0f)
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->defenseRootMontage, 1.2f, FName("DefenseStart"));
	}
	//바로 이동가능을 위해
	else
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->defenseInplaceMontage, 1.2f, FName("DefenseStart"));
	}
	
}

void UCharParryingState::Exit()
{
	Super::Exit();
	mainChar->GetCharStatusComponent()->SetMoveStop(false);
}
