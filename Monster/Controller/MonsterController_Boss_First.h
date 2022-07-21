// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterController_Boss.h"
#include "MonsterController_Boss_First.generated.h"

class UBlackboardData;
class UBehaviorTree;

UCLASS()
class LSH_API AMonsterController_Boss_First : public AMonsterController_Boss
{
	GENERATED_BODY()

public:
	AMonsterController_Boss_First();

private:
	UPROPERTY()
	UBlackboardData* BBAsset;
	UPROPERTY()
	UBehaviorTree* BTAsset;

protected:
	virtual void BeginPlay() override;
};
