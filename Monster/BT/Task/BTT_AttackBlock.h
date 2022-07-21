// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AttackBlock.generated.h"



UCLASS()
class LSH_API UBTT_AttackBlock : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_AttackBlock();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
