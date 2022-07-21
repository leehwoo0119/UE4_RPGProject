// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDefenseSuccessState.h"
#include "ProjectGameInstance.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "CharStatusComponent.h"

UCharDefenseSuccessState::UCharDefenseSuccessState()
{
	PrimaryComponentTick.bCanEverTick = true;

	defenseSuccessCurveLength = 0.4f;
}

void UCharDefenseSuccessState::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat DefenseSuccessCallback;
	FOnTimelineEventStatic DefenseSuccessFinishedCallback;

	DefenseSuccessCallback.BindUFunction(this, FName("DefenseSuccessStart"));
	DefenseSuccessFinishedCallback.BindUFunction(this, FName("DefenseSuccessEnd"));

	defenseSuccessCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetHitCurveFloat(), DefenseSuccessCallback);
	defenseSuccessCurveFTimeline.SetTimelineFinishedFunc(DefenseSuccessFinishedCallback);
	defenseSuccessCurveFTimeline.SetTimelineLength(defenseSuccessCurveLength);
}

void UCharDefenseSuccessState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	defenseSuccessCurveFTimeline.TickTimeline(DeltaTime);
}

void UCharDefenseSuccessState::Enter()
{
	Super::Enter();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Defense);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharUltimatePoint(10.0f);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharStaminaPoint(-mainChar->GetCharStatusComponent()->GetDamage());

	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(true);

	mainChar->GetCharStatusComponent()->SetMoveStop(true);
	mainChar->GetCharacterMovement()->MaxAcceleration = charController->GetCharHitMaxAcceleration();
	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}

void UCharDefenseSuccessState::Execute()
{
	Super::Execute();

	mainChar->GetCharacterMovement()->bOrientRotationToMovement = false;

	StartDefenseSuccessCurveFTimeline();

	mainChar->GetAnimInstance()->AnimMontagePlay(
		mainChar->GetAnimInstance()->defenseSuccessMontage,
		1.0f,
		FName("DefenseSuccessStart"));
}

void UCharDefenseSuccessState::Exit()
{
	Super::Exit();
}

void UCharDefenseSuccessState::StartDefenseSuccessCurveFTimeline()
{
	defenseSuccessCurveFTimeline.PlayFromStart();
}

void UCharDefenseSuccessState::DefenseSuccessStart()
{
	FVector direction = mainChar->GetActorForwardVector() * -1.0f;
	mainChar->AddMovementInput(direction, 1.0f);
}

void UCharDefenseSuccessState::DefenseSuccessEnd()
{
	mainChar->GetCharStatusComponent()->SetMoveStop(false);
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	mainChar->GetCharacterMovement()->MaxAcceleration = charController->GetCharNormalMaxAcceleration();

	if(mainChar->GetCharStatusComponent()->GetDefense())
		mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharWalkSpeed();
	else
		mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}