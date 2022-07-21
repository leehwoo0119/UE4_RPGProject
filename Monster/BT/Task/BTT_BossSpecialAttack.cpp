// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BossSpecialAttack.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBossCombatComponent.h"

UBTT_BossSpecialAttack::UBTT_BossSpecialAttack()
{
	NodeName = TEXT("BossSpecialAttack");
}

EBTNodeResult::Type UBTT_BossSpecialAttack::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossSpecialAttack aIMonster null Error!"));
		return EBTNodeResult::Failed;
	}

	UMonsterBossCombatComponent* bossCombatComp = Cast<UMonsterBossCombatComponent>(aIMonster->MON_COMBATCOMP);
	if (!bossCombatComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossSpecialAttack bossCombatComp null Error!"));
		return EBTNodeResult::Failed;
	}
	bossCombatComp->SpecialAttack();

	return EBTNodeResult::Succeeded;
}
