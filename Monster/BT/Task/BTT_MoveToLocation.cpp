// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToLocation.h"
#include "MonsterController_Normal.h"
#include "MonsterBase_Normal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTT_MoveToLocation::UBTT_MoveToLocation()
{
	NodeName = TEXT("MoveToLocation");
}

EBTNodeResult::Type UBTT_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToLocation myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}
	FVector getLocation = myBlackboard->GetValueAsVector("targetLocationKey");

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(_ownerComp.GetAIOwner(), getLocation);

	return EBTNodeResult::Succeeded;
}