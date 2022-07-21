// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckBossGroggyCount.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"

UBTD_CheckBossGroggyCount::UBTD_CheckBossGroggyCount()
{
	NodeName = TEXT("CheckBossGroggyCount");
}

bool UBTD_CheckBossGroggyCount::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckBossGroggyCount aIMonster null Error!"));
		return false;
	}

	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::Groggy) return false;

	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)return false;

	if (aIMonster->MON_STATUS->GetMaxGroggyCount() <= aIMonster->MON_STATUS->GetCurrentGroggyCount())
	{
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Groggy);

		return true;
	}

	return false;
}
