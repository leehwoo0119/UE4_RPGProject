// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckBossOffenserseRecorvery.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"

UBTD_CheckBossOffenserseRecorvery::UBTD_CheckBossOffenserseRecorvery()
{
	NodeName = TEXT("CheckBossOffenserseRecorvery");
}

bool UBTD_CheckBossOffenserseRecorvery::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckBossOffenserseRecorvery aIMonster null Error!"));
		return false;
	}
	
	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseRecorvery) return false;

	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::Attack ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::Groggy ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)return false;

	if (aIMonster->MON_STATUS->GetCheckOffenserseRecorvery())
	{
		float basePercent = 0.0f;

		if (aIMonster->MON_BOSSPHASESTATE == EAI_BossPhase_State::FirstPhase)
			basePercent = 0.3f;
		else if (aIMonster->MON_BOSSPHASESTATE == EAI_BossPhase_State::SecondPhase)
			basePercent = 0.15f;

		if (aIMonster->MON_STATUS->GetCurMonsterOffensersePoint() <= aIMonster->MON_STATUS->GetMaxMonsterOffensersePoint() * basePercent)
		{
			aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::OffenserseRecorvery);

			return true;
		}		
	}

	return false;
}
