// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DashAttack.h"
#include "MonsterController_Normal.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterCombatComponent.h"

UBTT_DashAttack::UBTT_DashAttack()
{
	NodeName = TEXT("DashAttack");
}

EBTNodeResult::Type UBTT_DashAttack::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	APawn* aiChar = _ownerComp.GetAIOwner()->GetPawn();
	if (!aiChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("DashAttack aiChar null Error!"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("DashAttack myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}

	AMonsterBase* castAIChar = Cast<AMonsterBase>(aiChar);
	if (!castAIChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("DashAttack castAIChar null Error!"));
		return EBTNodeResult::Failed;
	}
	UMonsterCombatComponent* monsterCombatComponent = castAIChar->MON_COMBATCOMP;
	if (!monsterCombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("DashAttack monsterCombatComponent null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterCombatComponent->SpecialSkill();
	return EBTNodeResult::Succeeded;
}

