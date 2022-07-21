// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeIdleState.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

UBTT_ChangeIdleState::UBTT_ChangeIdleState()
{
	NodeName = TEXT("ChangeIdleState");
}

EBTNodeResult::Type UBTT_ChangeIdleState::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("ExecuteTask"));

	monsterBaseController = Cast<AMonsterBaseController>(_ownerComp.GetAIOwner());
	if (!monsterBaseController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeIdleState monsterBaseController null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterBase = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!monsterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeIdleState monsterBase null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterBase->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	monsterBase->GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	monsterBase->SetActorRelativeRotation(monsterBase->GetStartRotation());
	monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Idle);
	return EBTNodeResult::Succeeded;
}
