// Fill out your copyright notice in the Description page of Project Settings.


#include "CharWeaponSwapState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharTargetLockOnComponent.h"
#include "MainCharWeaponSlot.h"
#include "CharStatusComponent.h"

UCharWeaponSwapState::UCharWeaponSwapState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharWeaponSwapState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharWeaponSwapState::Enter()
{
	Super::Enter();
	mainChar->GetCharTargetLockOnComponent()->ResetTarget();

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
		mainChar->GetMainCharWeaponSlot()->DettachArrowToHand();
}

void UCharWeaponSwapState::Execute()
{
	Super::Execute();

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetWeaponState(EWeaponState_Type::Unarmed);
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::WeaponSwap);
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::WeaponSwap);
			mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->weaponSwapMontage, 1.0f, FName("KatanaUnarmed"));
			break;
		case EKatanaState_Type::Crouch:
			mainChar->GetCharStatusComponent()->SetWeaponState(EWeaponState_Type::Unarmed);
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::CrouchWeaponSwap);
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::CrouchWeaponSwap);
			mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->weaponSwapMontage, 1.0f, FName("CrouchKatanaUnarmed"));
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetWeaponState(EWeaponState_Type::Unarmed);
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::WeaponSwap);
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::WeaponSwap);
			mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->weaponSwapMontage, 1.0f, FName("KatanaArmed"));
			break;
		case EBowState_Type::Crouch:
			mainChar->GetCharStatusComponent()->SetWeaponState(EWeaponState_Type::Unarmed);
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::CrouchWeaponSwap);
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::CrouchWeaponSwap);
			mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->weaponSwapMontage, 1.0f, FName("CrouchKatanaArmed"));
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void UCharWeaponSwapState::Exit()
{
	Super::Exit();
}