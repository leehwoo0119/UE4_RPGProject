// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDashState.h"
#include "MainCharController.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharWeaponSlot.h"
#include "CharStatusComponent.h"

UCharDashState::UCharDashState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharDashState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharDashState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharStaminaPoint(-mainChar->GetCharStatusComponent()->GetBehaviorStaminaCost());
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}

void UCharDashState::Execute()
{
	Super::Execute();
	
	//예외사항
	if ((!charController->GetMoveForward() && !charController->GetMoveRight()))
	{
		mainChar->GetCharStatusComponent()->SetDashState(EDashState_Type::BackDash);
	}

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Dash);
	mainChar->bUseControllerRotationYaw = true;
	switch (mainChar->GetCharStatusComponent()->GetDashState())
	{
	case EDashState_Type::FrontDash:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dashMontage, 1.0f, FName("FrontDash"));
		break;
	case EDashState_Type::BackDash:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dashMontage, 1.0f, FName("BackDash"));
		break;
	case EDashState_Type::LeftDash:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dashMontage, 1.0f, FName("LeftDash"));
		break;
	case EDashState_Type::RightDash:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dashMontage, 1.0f, FName("RightDash"));
		break;
	case EDashState_Type::Default:
		break;
	}
}

void UCharDashState::Exit()
{
	Super::Exit();
}
