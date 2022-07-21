// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStunState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "MainCharWeaponSlot.h"
#include "CharStatusComponent.h"

UCharStunState::UCharStunState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharStunState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharStunState::Enter()
{
	Super::Enter();
	
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
	mainChar->GetCharStatusComponent()->PlusStunCnt(true);
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
		mainChar->GetMainCharWeaponSlot()->DettachArrowToHand();
}

void UCharStunState::Execute()
{
	Super::Execute();
	
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::TakeDownDamage ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::TakeDownDamage) return;

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Stun);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Stun);

	mainChar->GetAnimInstance()->AnimMontagePlay(
		mainChar->GetAnimInstance()->takeDamageMontage,
		0.7f,
		FName("Stun"));
}

void UCharStunState::Exit()
{
	Super::Exit();

}