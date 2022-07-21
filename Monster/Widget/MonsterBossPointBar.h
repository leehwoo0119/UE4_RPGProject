// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterBossPointBar.generated.h"

class UMonsterHealthBar;
class UMonsterOffenserseBar;
class AMonsterBase;
class UMonsterCalHealthBar;
class UMonsterCalOffenserseBar;

UCLASS()
class LSH_API UMonsterBossPointBar : public UUserWidget
{
	GENERATED_BODY()

private:
	AMonsterBase* monsterBase;

protected:
	UPROPERTY(meta = (BindWidget))
	UMonsterHealthBar* MonsterHealthBar;
	UPROPERTY(meta = (BindWidget))
	UMonsterCalHealthBar* MonsterCalHealthBar;
	UPROPERTY(meta = (BindWidget))
	UMonsterOffenserseBar* MonsterOffenserseBar;
	UPROPERTY(meta = (BindWidget))
	UMonsterCalOffenserseBar* MonsterCalOffenserseBar;

public:
	UFUNCTION(BlueprintCallable)
	void SetMonsterBase(AMonsterBase* _monsterBase);
};
