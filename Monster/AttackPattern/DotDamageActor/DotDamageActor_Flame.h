// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DotDamageActor.h"
#include "DotDamageActor_Flame.generated.h"

UCLASS()
class LSH_API ADotDamageActor_Flame : public ADotDamageActor
{
	GENERATED_BODY()

public:
	ADotDamageActor_Flame();

protected:
	virtual void BeginPlay() override;

public:
	void FlameDotDamage();
};
