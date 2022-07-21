// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDefenseBrokenState.h"
#include "CharStatusComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"

UCharDefenseBrokenState::UCharDefenseBrokenState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharDefenseBrokenState::BeginPlay()
{
	Super::BeginPlay();

}

void UCharDefenseBrokenState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::DefenseBroken);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharStaminaPoint(-mainChar->GetCharStatusComponent()->GetDamage());
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}

void UCharDefenseBrokenState::Execute()
{
	Super::Execute();

	mainChar->GetAnimInstance()->AnimMontagePlay(
		mainChar->GetAnimInstance()->defenseRootMontage,
		1.0f,
		FName("DefenseBroken"));
}

void UCharDefenseBrokenState::Exit()
{
	Super::Exit();
}