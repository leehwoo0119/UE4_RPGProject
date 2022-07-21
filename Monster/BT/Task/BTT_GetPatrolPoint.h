// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetPatrolPoint.generated.h"


UCLASS()
class LSH_API UBTT_GetPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTT_GetPatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
