// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterCalHealthBar.generated.h"

class UProgressBar;
class AMonsterBase;

UCLASS()
class LSH_API UMonsterCalHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterBase", meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monsterBase;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* calHealthBar;

public:
	void SetMonsterBase(AMonsterBase* _monsterBase);
};
