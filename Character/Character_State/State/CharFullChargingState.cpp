// Fill out your copyright notice in the Description page of Project Settings.


#include "CharFullChargingState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharBowComponent.h"
#include "MainCharWeaponSlot.h"
#include "MainCharBow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharStatusComponent.h"

UCharFullChargingState::UCharFullChargingState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharFullChargingState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharFullChargingState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Shooting);
}

void UCharFullChargingState::Execute()
{
	Super::Execute();
	mainChar->GetCharBowComponent()->ReverseBowChargingCameraCurveFTimeline();
	mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->EndCharge();
	mainChar->GetCharBowComponent()->SetArrowShootDir(true);
	mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->bowAttackMontage, 1.0f, FName("BowShooting"));
}

void UCharFullChargingState::Exit()
{
	Super::Exit();
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	mainChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}