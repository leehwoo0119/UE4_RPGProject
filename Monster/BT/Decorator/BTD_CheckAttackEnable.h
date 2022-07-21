// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckAttackEnable.generated.h"


UCLASS()
class LSH_API UBTD_CheckAttackEnable : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckAttackEnable();

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;
};
