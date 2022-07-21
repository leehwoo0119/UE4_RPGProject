// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_OnInvestigate.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"
#include "MonsterCombatComponent.h"
#include "MonsterBaseStatus.h"
#include "LHW/PatrolPathInterface.h"
#include "LHW/PatrolPath.h"

UBTT_OnInvestigate::UBTT_OnInvestigate()
{
	NodeName = TEXT("OnInvestigate");
	investigateRange = 0.0f;
}

EBTNodeResult::Type UBTT_OnInvestigate::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	monsterBaseController = Cast<AMonsterBaseController>(_ownerComp.GetAIOwner());
	if (!monsterBaseController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnInvestigate monsterBaseController null Error!"));
		return EBTNodeResult::Failed;
	}
	myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnInvestigate myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterBase = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!monsterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnInvestigate monsterBase null Error!"));
		return EBTNodeResult::Failed;
	}
	UMonsterCombatComponent* monsterCombatComponent = monsterBase->MON_COMBATCOMP;
	monsterBase = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!monsterCombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnInvestigate monsterCombatComponent null Error!"));
		return EBTNodeResult::Failed;
	}
	monsterCombatComponent->LookAround(true);

	FVector monsterLoc = FVector(
			monsterBase->GetActorLocation().X, 
			monsterBase->GetActorLocation().Y, 
			0.0f);

	FVector targetLoc = FVector(
			myBlackboard->GetValueAsVector("targetLocationKey").X,
			myBlackboard->GetValueAsVector("targetLocationKey").Y,
			0.0f);

	float distance =
		FVector::Distance(
			monsterLoc,
			targetLoc);
	//UE_LOG(LogTemp, Log, TEXT("%d"), monsterBase->GetMonsterStatus()->GetInvestigateMoveStart());

	if (monsterBase->MON_STATUS->GetInvestigateMoveStart() &&
		monsterBase->GetVelocity().Size() == 0.0f)
	{
		ChangeIdleState();
	}
	else if (distance < investigateRange)
	{
		ChangeIdleState();
	}

	//속도가 0인경우는 못가는 위치에 핑이찍힘
	if (!monsterBase->MON_STATUS->GetInvestigateMoveStart() &&
		monsterBase->GetVelocity().Size() != 0.0f)
	{
		monsterBase->MON_STATUS->SetInvestigateMoveStart(true);
	}
	return EBTNodeResult::Succeeded;
}

void UBTT_OnInvestigate::ChangeIdleState()
{
	if (myBlackboard->GetValueAsObject("focusActorKey") == nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("ChangeIdleState"));
		
		monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::LookAround);
		// 몬스터 애니메이션 실행 중 이동, 회전 방지를 위한 코드
		UMonsterCombatComponent* monsterCombatComponent = monsterBase->MON_COMBATCOMP;
		monsterCombatComponent->LookAround();
	}
}

