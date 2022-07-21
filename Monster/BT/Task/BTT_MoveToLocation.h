// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveToLocation.generated.h"


UCLASS()
class LSH_API UBTT_MoveToLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveToLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
