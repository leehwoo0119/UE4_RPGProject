// Fill out your copyright notice in the Description page of Project Settings.


#include "CharAttackAvoidState.h"
#include "MainCharAnimInstance.h"
#include "MainChar.h"
#include "CharStatusComponent.h"
#include "LSH/MonsterBase.h"
#include "LSH/MonsterCombatComponent.h"
#include "CharTargetLockOnComponent.h"
#include "StealthCamera.h"
#include "Camera/CameraComponent.h"
#include "CharCombatComponent.h"
#include "Kismet/GameplayStatics.h"

UCharAttackAvoidState::UCharAttackAvoidState() 
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCharAttackAvoidState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharAttackAvoidState::Enter()
{
	Super::Enter();

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::AttackAvoid);
	StartExecutionCamera();
}

void UCharAttackAvoidState::Execute()
{
	Super::Execute();

	check(mainChar->GetCharStatusComponent()->GetAttackAvoidTarget() != nullptr);

	mainChar->GetCharTargetLockOnComponent()->TargetLockOn(mainChar->GetCharStatusComponent()->GetAttackAvoidTarget());
	switch (mainChar->GetCharStatusComponent()->GetAttackAvoid())
	{
	case EAttackAvoidState_Type::AttackAvoidSuccess:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackAvoidMontage, 1.15f, FName("AttackAvoidSuccess"));
		break;
	case EAttackAvoidState_Type::AttackAvoidFail:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->attackAvoidMontage, 1.15f, FName("AttackAvoid"));
		break;
	}
	mainChar->GetCharStatusComponent()->GetAttackAvoidTarget()->MON_COMBATCOMP->ExecuteAttackAvoid();
}

void UCharAttackAvoidState::Exit()
{
	Super::Exit();

	ResetCameraRot();
	mainChar->GetCharStatusComponent()->SetAttackAvoidTarget(nullptr);
}

void UCharAttackAvoidState::StartExecutionCamera()
{
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetStealthCamera()->GetOwner(), 0.3f);
}

void UCharAttackAvoidState::ResetCameraRot()
{
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	check(camManager != nullptr);
	FRotator resetCameraRot = FRotator(
		camManager->GetCameraRotation().Pitch,
		mainChar->GetActorRotation().Yaw,
		camManager->GetCameraRotation().Roll);
	UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(resetCameraRot);

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCamera()->GetOwner(), 0.3f);
}