// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckMaxChaseRange.generated.h"


UCLASS()
class LSH_API UBTD_CheckMaxChaseRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_CheckMaxChaseRange();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float maxChaseRange;

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;

};
