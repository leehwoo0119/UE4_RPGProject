// Fill out your copyright notice in the Description page of Project Settings.


#include "CharInstantDeathState.h"
#include "MainChar.h"
#include "CharStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InstantDeathCamera.h"
#include "Camera/CameraComponent.h"
#include "CharCombatComponent.h"

UCharInstantDeathState::UCharInstantDeathState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharInstantDeathState::BeginPlay()
{
	Super::BeginPlay();

}

void UCharInstantDeathState::Enter()
{
	Super::Enter();

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Die);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Die);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCombatComponent()->GetCharInstantDeathCamera()->GetUltimateAttackCamera()->GetOwner(), 0.3f);
}

void UCharInstantDeathState::Execute()
{
	Super::Execute();

	mainChar->GetCharacterMovement()->StopMovementImmediately();
	mainChar->GetAnimInstance()->AnimMontagePlay(
		mainChar->GetAnimInstance()->dieMontage, 0.7f, FName("InstantDeath"));
}

void UCharInstantDeathState::Exit()
{
	Super::Exit();

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCamera()->GetOwner(), 0.3f);
}