// Fill out your copyright notice in the Description page of Project Settings.


#include "CharRecoveryState.h"
#include "MainChar.h"
#include "CharCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "CharNormalState.h"
#include "CharStateMachine.h"
#include "CharStateActor.h"
#include "CharStatusComponent.h"

UCharRecoveryState::UCharRecoveryState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharRecoveryState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharRecoveryState::Enter()
{
	Super::Enter();
	
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::HealthRecovery);
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::HealthRecovery);

	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}

void UCharRecoveryState::Execute()
{
	Super::Execute();

	UGameplayStatics::SpawnEmitterAttached(
		mainChar->GetCharCombatComponent()->GetRecoveryParticle(),
		mainChar->GetMesh());

	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharWalkSpeed();
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharHealthPoint(50.0f);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharRecoveryPoint(0.0f, true);

	FTimerHandle changeNormalStateTimer;
	GetWorld()->GetTimerManager().SetTimer(changeNormalStateTimer, FTimerDelegate::CreateLambda([&]()
	{
			//만약 상태전환이 이뤄지지 않은 상태라면
			if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::HealthRecovery ||
				mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::HealthRecovery)
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			}

	}), 2.0f, false);
}

void UCharRecoveryState::Exit()
{
	Super::Exit();

	mainChar->GetCharacterMovement()->MaxWalkSpeed = charController->GetCharRunSpeed();
}