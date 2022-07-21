// Fill out your copyright notice in the Description page of Project Settings.


#include "CharBowChargingState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharBowComponent.h"
#include "MainCharWeaponSlot.h"
#include "MainCharBow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharController.h"
#include "CharStatusComponent.h"

UCharBowChargingState::UCharBowChargingState()
{

}

void UCharBowChargingState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharBowChargingState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Aiming);
	mainChar->GetMainCharWeaponSlot()->DettachArrowToHand();
}

void UCharBowChargingState::Execute()
{
	Super::Execute();
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = false;
	mainChar->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	mainChar->GetCharSpringArm()->bEnableCameraLag = false;
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharWalkSpeed();

	mainChar->GetCharBowComponent()->StartBowChargingCameraCurveFTimeline();
	mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->StartCharge();
	mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->bowAttackMontage, 1.0f, FName("BowChargingStart"));
}

void UCharBowChargingState::Exit()
{
	Super::Exit();
	mainChar->GetCharSpringArm()->bEnableCameraLag = true;
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}