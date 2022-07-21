// Fill out your copyright notice in the Description page of Project Settings.


#include "CharCheckAttackAvoidState.h"
#include "MainChar.h"
#include "CharStatusComponent.h"

UCharCheckAttackAvoidState::UCharCheckAttackAvoidState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharCheckAttackAvoidState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharCheckAttackAvoidState::Enter()
{
	Super::Enter();

	mainChar->GetWorldTimerManager().ClearTimer(attackAvoidSuccessTimer);
	mainChar->GetWorldTimerManager().ClearTimer(attackAvoidFailTimer);
	mainChar->GetWorldTimerManager().ClearTimer(attackAvoidDefaultTimer);
	mainChar->GetCharStatusComponent()->SetAttackAvoid(EAttackAvoidState_Type::AttackAvoidFail);
}

void UCharCheckAttackAvoidState::Execute()
{
	Super::Execute();

	GetWorld()->GetTimerManager().SetTimer(attackAvoidSuccessTimer, FTimerDelegate::CreateLambda([&]()
		{
			mainChar->GetCharStatusComponent()->SetAttackAvoid(EAttackAvoidState_Type::AttackAvoidSuccess);
			SetFailState();
	
		}), 0.05f, false);
}

void UCharCheckAttackAvoidState::Exit()
{
	Super::Exit();

}

void UCharCheckAttackAvoidState::SetFailState()
{
	GetWorld()->GetTimerManager().SetTimer(attackAvoidFailTimer, FTimerDelegate::CreateLambda([&]()
		{
			mainChar->GetCharStatusComponent()->SetAttackAvoid(EAttackAvoidState_Type::AttackAvoidFail);
			SetDefaultState();

		}), 0.13f, false);
}

void UCharCheckAttackAvoidState::SetDefaultState()
{
	GetWorld()->GetTimerManager().SetTimer(attackAvoidFailTimer, FTimerDelegate::CreateLambda([&]()
		{
			mainChar->GetCharStatusComponent()->SetAttackAvoid(EAttackAvoidState_Type::Default);

		}), 0.1f, false);
}
