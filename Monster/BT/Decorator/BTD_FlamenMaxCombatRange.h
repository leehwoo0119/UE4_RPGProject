// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_FlamenMaxCombatRange.generated.h"

UCLASS()
class LSH_API UBTD_FlamenMaxCombatRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_FlamenMaxCombatRange();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float maxCombatRange;

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;

};
