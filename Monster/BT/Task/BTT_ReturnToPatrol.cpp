// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ReturnToPatrol.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterWidgetComponent.h"
#include "MonsterBaseStatus.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

UBTT_ReturnToPatrol::UBTT_ReturnToPatrol()
{
	NodeName = TEXT("ReturnToPatrol");
}

EBTNodeResult::Type UBTT_ReturnToPatrol::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	monsterBaseController = Cast<AMonsterBaseController>(_ownerComp.GetAIOwner());
	if (!monsterBaseController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReturnToPatrol monsterBaseController null Error!"));
		return EBTNodeResult::Failed;
	}
	myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReturnToPatrol myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterBase = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReturnToPatrol monsterBase null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterBase->GetStealthArea()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	monsterBaseController->ClearFocus(EAIFocusPriority::Gameplay);
	myBlackboard->SetValueAsObject("focusActorKey", nullptr);
	myBlackboard->SetValueAsVector("targetLocationKey", monsterBase->GetStartLocation());
	monsterBase->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
	monsterBase->MON_STATUS->SetInvestigateMoveStart(false);
	monsterBase->MON_WIDGETCOMP->SetMonsterStateImageVisible(true);
	monsterBase->GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	monsterBase->GetMonsterPointBarWidget()->SetVisibility(false);

	return EBTNodeResult::Succeeded;
}
