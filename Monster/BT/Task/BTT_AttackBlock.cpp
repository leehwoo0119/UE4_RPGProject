// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackBlock.h"
#include "MonsterController_Normal.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterCombatComponent.h"
#include "MonsterStateMachine.h"

UBTT_AttackBlock::UBTT_AttackBlock() 
{
	NodeName = TEXT("AttackBlock");
}

EBTNodeResult::Type UBTT_AttackBlock::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	APawn* aiChar = _ownerComp.GetAIOwner()->GetPawn();
	if (!aiChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackBlock aiChar null Error!"));
		return EBTNodeResult::Failed;
	}
	
	AMonsterBase* castAIChar = Cast<AMonsterBase>(aiChar);
	if (!castAIChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackBlock castAIChar null Error!"));
		return EBTNodeResult::Failed;
	}
	UMonsterCombatComponent* monsterCombatComponent = castAIChar->MON_COMBATCOMP;
	if (!monsterCombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackBlock monsterCombatComponent null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterCombatComponent->AttackBlock();

	return EBTNodeResult::Succeeded;
}
