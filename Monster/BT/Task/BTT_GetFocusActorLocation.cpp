// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetFocusActorLocation.h"
#include "MonsterController_Normal.h"
#include "MonsterBase_Normal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"

UBTT_GetFocusActorLocation::UBTT_GetFocusActorLocation()
{
	NodeName = TEXT("GetFocusActorLocation");
}

EBTNodeResult::Type UBTT_GetFocusActorLocation::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetFocusActorLocation myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}
	auto playerChar = myBlackboard->GetValueAsObject("focusActorKey");
	if (!playerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetFocusActorLocation playerChar null Error!"));
		return EBTNodeResult::Failed;
	}
	
	APawn* castPlayerChar = Cast<APawn>(playerChar);
	if (!castPlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetFocusActorLocation castPlayerChar null Error!"));
		return EBTNodeResult::Failed;
	}

	myBlackboard->SetValueAsVector("targetLocationKey", castPlayerChar->GetActorLocation());

	return EBTNodeResult::Succeeded;
}