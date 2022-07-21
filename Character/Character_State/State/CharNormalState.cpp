// Fill out your copyright notice in the Description page of Project Settings.


#include "CharNormalState.h"
#include "CharStatusComponent.h"
#include "MainChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "CharCombatComponent.h"

UCharNormalState::UCharNormalState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharNormalState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharNormalState::Enter()
{
	Super::Enter();
	
	mainChar->GetCharStatusComponent()->SetMoveStop(false);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Normal);
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Normal);
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (!mainChar->GetCharStatusComponent()->GetStaminaPointRecovery())
	{
		float staminaRecoveryDelay = 0.05f;

		//이전 상태가 가드 깨짐 상태면 회복속도 상승
		if (mainChar->GetCharStatusComponent()->GetPrevKatanaState() == EKatanaState_Type::DefenseBroken)
		{
			staminaRecoveryDelay = 0.03f;
		}
		mainChar->GetCharCombatComponent()->StaminaPointRecoveryStart(staminaRecoveryDelay);
		mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(true);
	}		

	mainChar->UnCrouch();
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}

void UCharNormalState::Execute()
{
	Super::Execute();
}

void UCharNormalState::Exit()
{
	Super::Exit();
}
