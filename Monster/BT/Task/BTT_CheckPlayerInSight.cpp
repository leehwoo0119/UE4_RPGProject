// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CheckPlayerInSight.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"

UBTT_CheckPlayerInSight::UBTT_CheckPlayerInSight()
{
	NodeName = TEXT("CheckPlayerInSight");
	sightTimer = 5;
}

EBTNodeResult::Type UBTT_CheckPlayerInSight::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	monsterBaseController = Cast<AMonsterBaseController>(_ownerComp.GetAIOwner());
	myBlackboard = _ownerComp.GetBlackboardComponent();
	monsterBase = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());

	if (myBlackboard->GetValueAsBool("bIsCanSeePlayer"))
	{
		sightTimer = 5;
	}
	else
	{
		sightTimer--;

		if (sightTimer <= 0)
		{
			monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::ReturnPatrol);
		}
	}

	return EBTNodeResult::Succeeded;
}