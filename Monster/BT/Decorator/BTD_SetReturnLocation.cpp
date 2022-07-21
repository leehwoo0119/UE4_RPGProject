// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_SetReturnLocation.h"
#include "LHW/PatrolPathInterface.h"
#include "LHW/PatrolPath.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_SetReturnLocation::UBTD_SetReturnLocation()
{
	NodeName = TEXT("SetReturnLocation");
}

bool UBTD_SetReturnLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetReturnLocation UBlackboardComponent null Error!"));
		return false;
	}
	AMonsterBase* monsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!monsterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetReturnLocation monsterBase null Error!"));
		return false;
	}

	IPatrolPathInterface* patrolPath = Cast<IPatrolPathInterface>(monsterBase);
	if (patrolPath->GetPatrolPath())
		myBlackboard->SetValueAsVector("returnLocationKey", patrolPath->GetPatrolPath()->GetNextPointWorldPos());
	else
		myBlackboard->SetValueAsVector("returnLocationKey", monsterBase->GetStartLocation());

	return true;
}