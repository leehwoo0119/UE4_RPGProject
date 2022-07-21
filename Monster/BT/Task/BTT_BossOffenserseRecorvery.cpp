// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BossOffenserseRecorvery.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBossCombatComponent_First.h"

UBTT_BossOffenserseRecorvery::UBTT_BossOffenserseRecorvery()
{
	NodeName = TEXT("BossOffenserseRecorvery");
}

EBTNodeResult::Type UBTT_BossOffenserseRecorvery::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossOffenserseRecorvery aIMonster null Error!"));
		return EBTNodeResult::Failed;
	}

	UMonsterBossCombatComponent_First* bossCombatComp = Cast<UMonsterBossCombatComponent_First>(aIMonster->MON_COMBATCOMP);
	if (!bossCombatComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossOffenserseRecorvery bossCombatComp null Error!"));
		return EBTNodeResult::Failed;
	}
	bossCombatComp->OffenserseRecorvery();

	return EBTNodeResult::Succeeded;
}
