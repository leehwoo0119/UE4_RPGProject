// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDefenseState.h"
#include "MainCharController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharStatusComponent.h"

UCharDefenseState::UCharDefenseState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharDefenseState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharDefenseState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Defense);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}

void UCharDefenseState::Execute()
{
	Super::Execute();
	if (!mainChar->GetCharStatusComponent()->GetMoveStop())
		mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharWalkSpeed();		
}

void UCharDefenseState::Exit()
{
	Super::Exit();
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}
