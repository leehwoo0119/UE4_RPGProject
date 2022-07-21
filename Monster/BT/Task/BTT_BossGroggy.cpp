// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BossGroggy.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBossCombatComponent_First.h"

UBTT_BossGroggy::UBTT_BossGroggy()
{
	NodeName = TEXT("BossGroggy");
}

EBTNodeResult::Type UBTT_BossGroggy::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("BossGroggy aIMonster null Error!"));
		return EBTNodeResult::Failed;
	}

	UMonsterBossCombatComponent_First* bossCombatComp = Cast<UMonsterBossCombatComponent_First>(aIMonster->MON_COMBATCOMP);
	bossCombatComp->Groggy();

	return EBTNodeResult::Succeeded;
}