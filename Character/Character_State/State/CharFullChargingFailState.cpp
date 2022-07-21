// Fill out your copyright notice in the Description page of Project Settings.


#include "CharFullChargingFailState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharBowComponent.h"
#include "MainCharWeaponSlot.h"
#include "MainCharBow.h"
#include "GameFramework/CharacterMovementComponent.h"

UCharFullChargingFailState::UCharFullChargingFailState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharFullChargingFailState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharFullChargingFailState::Enter()
{
	Super::Enter();
}

void UCharFullChargingFailState::Execute()
{
	Super::Execute();
	mainChar->GetCharBowComponent()->ReverseBowChargingCameraCurveFTimeline();
	mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->EndCharge();
	mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->bowAttackMontage, 1.0f, FName("BowChargingCancel"));
}

void UCharFullChargingFailState::Exit()
{
	Super::Exit();
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	mainChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}