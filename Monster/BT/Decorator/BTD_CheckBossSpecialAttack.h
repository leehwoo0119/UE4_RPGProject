// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckBossSpecialAttack.generated.h"


UCLASS()
class LSH_API UBTD_CheckBossSpecialAttack : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckBossSpecialAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIgnoreBossPhase;

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;
};
