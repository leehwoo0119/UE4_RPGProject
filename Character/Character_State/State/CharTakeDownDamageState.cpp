// Fill out your copyright notice in the Description page of Project Settings.


#include "CharTakeDownDamageState.h"
#include "CharStatusComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "MainCharWeaponSlot.h"

UCharTakeDownDamageState::UCharTakeDownDamageState()
{
	PrimaryComponentTick.bCanEverTick = true;

	takeDownDamageCurveLength = 0.4f;
}

void UCharTakeDownDamageState::BeginPlay()
{
	Super::BeginPlay();

	check(mainChar->GetCharCombatComponent()->GetHitCurveFloat() != nullptr);
	FOnTimelineFloat TakeDownDamageCallback;

	TakeDownDamageCallback.BindUFunction(this, FName("TakeDownDamagStart"));

	takeDownDamageCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetHitCurveFloat(), TakeDownDamageCallback);
	takeDownDamageCurveFTimeline.SetTimelineLength(takeDownDamageCurveLength);
}

void UCharTakeDownDamageState::Enter()
{
	Super::Enter();

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
		mainChar->GetMainCharWeaponSlot()->DettachArrowToHand();

	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
	mainChar->GetCharStatusComponent()->SetMoveStop(true);
}

void UCharTakeDownDamageState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	takeDownDamageCurveFTimeline.TickTimeline(DeltaTime);
}

void UCharTakeDownDamageState::Execute()
{
	Super::Execute();

	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::TakeDownDamage ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::TakeDownDamage)
		return;

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::TakeDownDamage);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::TakeDownDamage);

	
	mainChar->GetCharacterMovement()->MaxAcceleration = charController->GetCharHitMaxAcceleration() * 10.0f;
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharSprintSpeed();

	if (mainChar->GetCharStatusComponent()->GetDamageDirection() >= -90.0f && 
		mainChar->GetCharStatusComponent()->GetDamageDirection() <= 90.0f)
	{
		mainChar->GetCharacterMovement()->bOrientRotationToMovement = false;
		StartTakeDownDamageCurveFTimeline();

		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDownDamageFrontMontage,
			1.0f,
			FName("TakeDownDamageStart_Front"));
	}
	else
	{
		mainChar->GetAnimInstance()->AnimMontagePlay(
			mainChar->GetAnimInstance()->takeDownDamageBackMontage,
			1.0f,
			FName("TakeDownDamageStart_Back"));
	}
}

void UCharTakeDownDamageState::Exit()
{
	Super::Exit();

}

void UCharTakeDownDamageState::StartTakeDownDamageCurveFTimeline()
{
	takeDownDamageCurveFTimeline.PlayFromStart();
}

void UCharTakeDownDamageState::TakeDownDamagStart()
{
	FVector direction = mainChar->GetActorForwardVector() * -1.0f;
	mainChar->AddMovementInput(direction, 1.0f);
}

void UCharTakeDownDamageState::TakeDownDamagEnd()
{
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	mainChar->GetCharacterMovement()->MaxAcceleration = charController->GetCharNormalMaxAcceleration();
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}


