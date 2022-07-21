// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharBowAnimInstance.generated.h"

UCLASS()
class LHW_API UMainCharBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Gauge", meta = (AllowPrivateAccess = "true"))
	float chargeGauge;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetChargeGauge(float _val);
};
