// Fill out your copyright notice in the Description page of Project Settings.


#include "CharMeleeAttackState.h"
#include "ProjectGameInstance.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharStatusComponent.h"

UCharMeleeAttackState::UCharMeleeAttackState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharMeleeAttackState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharMeleeAttackState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::MeleeAttack);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}


void UCharMeleeAttackState::Execute()
{
	Super::Execute();

	switch (mainChar->GetCharStatusComponent()->GetMeleeAttackCnt())
	{
	case 0:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackMontage, 1.15f, FName("MeleeAttack_1"));
		break;
	case 1:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackMontage, 1.15f, FName("MeleeAttack_2"));
		break;
	case 2:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackMontage, 1.15f, FName("MeleeAttack_3"));
		break;
	case 3:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackMontage, 1.15f, FName("MeleeAttack_4"));
		break;
	default:
		break;
	}

	mainChar->GetCharStatusComponent()->PlusMeleeAttackCnt();
}

void UCharMeleeAttackState::Exit()
{
	Super::Exit();
}

