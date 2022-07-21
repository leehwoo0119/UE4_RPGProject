// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_OnInvestigate.generated.h"

class AMonsterBaseController;
class AMonsterBase;
class UBlackboardComponent;
struct FTimerHandle;

UCLASS()
class LSH_API UBTT_OnInvestigate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_OnInvestigate();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

	void ChangeIdleState();

private:
	AMonsterBaseController* monsterBaseController;
	AMonsterBase* monsterBase;
	UBlackboardComponent* myBlackboard;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float investigateRange;
};
