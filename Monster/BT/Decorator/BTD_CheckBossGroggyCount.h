// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckBossGroggyCount.generated.h"


UCLASS()
class LSH_API UBTD_CheckBossGroggyCount : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckBossGroggyCount();

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;
};
