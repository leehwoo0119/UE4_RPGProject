// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckBossSpecialAttack.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "Kismet/KismetMathLibrary.h"

UBTD_CheckBossSpecialAttack::UBTD_CheckBossSpecialAttack()
{
	NodeName = TEXT("CheckBossSpecialAttack");
}

bool UBTD_CheckBossSpecialAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckBossSpecialAttack aIMonster null Error!"));
		return false;
	}

	if (aIMonster->MON_BOSSPHASESTATE != EAI_BossPhase_State::SecondPhase && 
		!bIgnoreBossPhase) return false;

	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill) return false;
	
	if (aIMonster->MON_COMBATSTATE == EAI_Combat_State::Attack ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseRecorvery ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::Groggy ||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero||
		aIMonster->MON_COMBATSTATE == EAI_Combat_State::InstantKill)return false;

	float healthPercentage = aIMonster->MON_STATUS->GetCurMonsterHealthPoint() / aIMonster->MON_STATUS->GetMaxMonsterHealthPoint();

	if (UKismetMathLibrary::InRange_FloatFloat(healthPercentage, 0.5f, 0.75f, true, false) &&
		aIMonster->MON_STATUS->GetSpecialAttack(0) == false)
	{
		aIMonster->MON_STATUS->SetTrueSpecialAttack(0);
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::SpecialSkill);		
		aIMonster->MON_STATEMACHINE->SetBossSpecialAttackState(EAI_BossSpecialAttack_State::SlashProjection);
		return true;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(healthPercentage, 0.25f, 0.5f, true, false) &&
		aIMonster->MON_STATUS->GetSpecialAttack(1) == false)
	{
		aIMonster->MON_STATUS->SetTrueSpecialAttack(1);
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::SpecialSkill);
		aIMonster->MON_STATEMACHINE->SetBossSpecialAttackState(EAI_BossSpecialAttack_State::SlashProjection);
		return true;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(healthPercentage, 0.0f, 0.25f, true, false) &&
		aIMonster->MON_STATUS->GetSpecialAttack(2) == false)
	{
		aIMonster->MON_STATUS->SetTrueSpecialAttack(2);
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::SpecialSkill);
		aIMonster->MON_STATEMACHINE->SetBossSpecialAttackState(EAI_BossSpecialAttack_State::SlashProjection);
		return true;
	}

	return false;
}
