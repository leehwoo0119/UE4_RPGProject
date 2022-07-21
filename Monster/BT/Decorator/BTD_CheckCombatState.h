// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckCombatState.generated.h"


UCLASS()
class LSH_API UBTD_CheckCombatState : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_CheckCombatState();

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;
};
