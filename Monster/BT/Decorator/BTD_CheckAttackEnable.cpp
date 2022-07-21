// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttackEnable.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterBaseStatus.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"

UBTD_CheckAttackEnable::UBTD_CheckAttackEnable()
{
	NodeName = TEXT("CheckAttackEnable");
}

bool UBTD_CheckAttackEnable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	AMainChar* mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!aIMonster || !mainChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckAttackEnable aIMonster or mainChar null Error!"));
		return false;
	}

	if (aIMonster->MON_STATEMACHINE->GetCombatState() == EAI_Combat_State::Attack ||
		aIMonster->MON_STATEMACHINE->GetCombatState() == EAI_Combat_State::Hit)
	{
		return false;
	}

	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Die ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Die ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Execution ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Stealth)
	{
		return false;
	}

	if (aIMonster->MON_STATUS->GetAttackEnable())
	{
		aIMonster->MON_STATUS->SetAttackEnable(false);

		return true;
	}

	return false;
}
