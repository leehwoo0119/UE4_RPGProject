// Fill out your copyright notice in the Description page of Project Settings.


#include "CharTakeDamageState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "MainCharWeaponSlot.h"
#include "CharStatusComponent.h"

UCharTakeDamageState::UCharTakeDamageState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharTakeDamageState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharTakeDamageState::Enter()
{
	Super::Enter();

	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
		mainChar->GetMainCharWeaponSlot()->DettachArrowToHand();
}


void UCharTakeDamageState::Execute()
{
	Super::Execute();
	
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::TakeDownDamage ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::TakeDownDamage ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Stun ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Stun) return;

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::TakeDamage);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::TakeDamage);

	if (mainChar->GetCharStatusComponent()->GetDamageDirection() >= -135.0f && 
		mainChar->GetCharStatusComponent()->GetDamageDirection() < -45.0f)
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDamageMontage,
			1.0f,
			FName("Hit_Right"));
	}
	else if (mainChar->GetCharStatusComponent()->GetDamageDirection() >= -45.0f && 
			mainChar->GetCharStatusComponent()->GetDamageDirection() < 45.0f)
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDamageMontage,
			1.0f,
			FName("Hit_Front"));
	}
	else if (mainChar->GetCharStatusComponent()->GetDamageDirection() >= 45.0f && 
			mainChar->GetCharStatusComponent()->GetDamageDirection() < 135.0f)
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDamageMontage,
			1.0f,
			FName("Hit_Left"));
	}
	else
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDamageMontage,
			1.0f,
			FName("Hit_Back"));
	}
}

void UCharTakeDamageState::Exit()
{
	Super::Exit();
}