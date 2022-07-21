// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_WanderToPlayerSide.generated.h"

UCLASS()
class LSH_API UBTT_WanderToPlayerSide : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_WanderToPlayerSide();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
