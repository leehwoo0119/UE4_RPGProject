// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DotDamageActor.h"
#include "DotDamageActor_Poison.generated.h"

UCLASS()
class LSH_API ADotDamageActor_Poison : public ADotDamageActor
{
	GENERATED_BODY()

public:
	ADotDamageActor_Poison();

protected:
	virtual void BeginPlay() override;

public:
	void PoisonDotDamage();

	void PoisonDamageStart();
};
