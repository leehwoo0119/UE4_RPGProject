// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChangeIdleState.generated.h"

class AMonsterBaseController;
class AMonsterBase;
class UBlackboardComponent;

UCLASS()
class LSH_API UBTT_ChangeIdleState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ChangeIdleState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

private:
	AMonsterBaseController* monsterBaseController;
	AMonsterBase* monsterBase;
	UBlackboardComponent* myBlackboard;
};
