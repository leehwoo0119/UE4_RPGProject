// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "MonsterBase_Boss.generated.h"

class UMonster_Boss_WidgetComponent;

UCLASS()
class LSH_API AMonsterBase_Boss : public AMonsterBase
{
	GENERATED_BODY()

public:
	AMonsterBase_Boss();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetBossWidgetComponent(UMonster_Boss_WidgetComponent* _comp);

};
