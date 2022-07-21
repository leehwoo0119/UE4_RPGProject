// Fill out your copyright notice in the Description page of Project Settings.


#include "CharMeleeAttackStartState.h"
#include "CharStatusComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"

UCharMeleeAttackStartState::UCharMeleeAttackStartState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharMeleeAttackStartState::BeginPlay()
{
	Super::BeginPlay();	
}

void UCharMeleeAttackStartState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::MeleeAttackStart);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
	mainChar->GetCharStatusComponent()->ResetMeleeAttack();
}


void UCharMeleeAttackStartState::Execute()
{
	Super::Execute();

	switch (mainChar->GetCharStatusComponent()->GetMeleeAttackCnt())
	{
	case 0:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackMontage, 1.15f, FName("MeleeAttack_1"));
		break;
	default:
		break;
	}

	mainChar->GetCharStatusComponent()->PlusMeleeAttackCnt();
}

void UCharMeleeAttackStartState::Exit()
{
	Super::Exit();
}
