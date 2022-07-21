// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeAttackState.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "MonsterStateMachine.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChangeAttackState::UBTT_ChangeAttackState()
{
	NodeName = TEXT("ChangeAttackState");
}

EBTNodeResult::Type UBTT_ChangeAttackState::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeAttackState aIMonster null Error!"));
		return EBTNodeResult::Failed;
	}
	if(aIMonster->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::ReturnPatrol)return EBTNodeResult::Failed;

	APawn* castPlayerChar = Cast<APawn>(_ownerComp.GetBlackboardComponent()->GetValueAsObject("focusActorKey"));
	if (!castPlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeAttackState castPlayerChar null Error!"));
		return EBTNodeResult::Failed;
	}

	_ownerComp.GetAIOwner()->SetFocus(castPlayerChar);

	switch (aIMonster->MON_COMBATSTATE)
	{
	case EAI_Combat_State::Wait:
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Attack);
		break;
	case EAI_Combat_State::DefenseEnd:
		aIMonster->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Attack);
		break;
	}

	return EBTNodeResult::Succeeded;
}

