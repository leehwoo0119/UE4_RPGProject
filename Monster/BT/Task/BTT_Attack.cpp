// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "MonsterController_Normal.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterCombatComponent.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	APawn* aiChar = _ownerComp.GetAIOwner()->GetPawn();
	if (!aiChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack aiChar null Error!"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}

	EAI_MeleeAttack_State aiCombatState = static_cast<EAI_MeleeAttack_State>(myBlackboard->GetValueAsEnum("enum_AIMeleeAttackState"));

	AMonsterBase* castAIChar = Cast<AMonsterBase>(aiChar);
	if (!castAIChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack castAIChar null Error!"));
		return EBTNodeResult::Failed;
	}

	UMonsterCombatComponent* monsterCombatComponent = castAIChar->MON_COMBATCOMP;
	monsterCombatComponent->Attack(aiCombatState);
	return EBTNodeResult::Succeeded;
}

