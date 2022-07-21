// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossSpecialAttack.generated.h"


UCLASS()
class LSH_API UBTT_BossSpecialAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_BossSpecialAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
};
