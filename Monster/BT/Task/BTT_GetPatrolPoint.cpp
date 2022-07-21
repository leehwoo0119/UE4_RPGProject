// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetPatrolPoint.h"
#include "MonsterController_Normal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "LHW/PatrolPathInterface.h"
#include "LHW/PatrolPath.h"
#include "MonsterPointBar.h"
#include "MonsterBase.h"
#include "Components/WidgetComponent.h"

UBTT_GetPatrolPoint::UBTT_GetPatrolPoint()
{
	NodeName = TEXT("GetPatrolPoint");
}

EBTNodeResult::Type UBTT_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	APawn* aiChar = _ownerComp.GetAIOwner()->GetPawn();
	if (!aiChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPatrolPoint aiChar null Error!"));
		return EBTNodeResult::Failed;
	}

	AMonsterBase* castAIChar = Cast<AMonsterBase>(aiChar);
	if (!castAIChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPatrolPoint castAIChar null Error!"));
		return EBTNodeResult::Failed;
	}
	IPatrolPathInterface* patrolPath = Cast<IPatrolPathInterface>(castAIChar);

	if (castAIChar->GetMonsterPointBarWidget()->IsVisible())
	{
		castAIChar->GetMonsterPointBarWidget()->SetVisibility(false);
	}
	
	if (patrolPath->GetPatrolPath() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
		myBlackboard->SetValueAsVector("targetLocationKey",	patrolPath->GetPatrolPath()->GetNextPointWorldPos());

		patrolPath->GetPatrolPath()->IncreasePath();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}