// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDieState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharTargetLockOnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharStatusComponent.h"
#include "ProjectGameInstance.h"
#include "LSH/DotDamageManager.h"

UCharDieState::UCharDieState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharDieState::BeginPlay()
{
	Super::BeginPlay();
}

void UCharDieState::Enter()
{
	Super::Enter();


	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetIgnoreMoveInput(true);

	UProjectGameInstance* gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameIS->StopBackGroundSound();
	mainChar->CustomTimeDilation = 1.0f;

	mainChar->GetDotDamageManager()->AllDotDamageActorDestroy();
	mainChar->GetCharTargetLockOnComponent()->ResetTarget();
	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Die);
	mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Die);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
}

void UCharDieState::Execute()
{
	Super::Execute();

	int randVal = FMath::RandRange(0, 3);
	switch (randVal)
	{
	case 0:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dieMontage, 0.9f, FName("Die_1"));
		break;
	case 1:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dieMontage, 0.9f, FName("Die_2"));
		break;
	case 2:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dieMontage, 0.9f, FName("Die_3"));
		break;
	case 3:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->dieMontage, 0.9f, FName("Die_4"));
		break;
	default:
		break;
	}
}

void UCharDieState::Exit()
{
	Super::Exit();
}
