// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DotDamageActor.h"
#include "DotDamageActor_Frozen.generated.h"

/**
 *
 */
UCLASS()
class LSH_API ADotDamageActor_Frozen : public ADotDamageActor
{
	GENERATED_BODY()

public:
	ADotDamageActor_Frozen();

protected:
	virtual void BeginPlay() override;

public:
	void FrozenStack();

	void FrozenCrowdControl();
};
