// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckPlayerInSight.generated.h"

class AMonsterBaseController;
class AMonsterBase;
class UBlackboardComponent;

UCLASS()
class LSH_API UBTT_CheckPlayerInSight : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_CheckPlayerInSight();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

private:
	AMonsterBaseController* monsterBaseController;
	AMonsterBase* monsterBase;
	UBlackboardComponent* myBlackboard;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int sightTimer;

};
