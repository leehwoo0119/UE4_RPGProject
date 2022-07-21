// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterOffenserseBar.generated.h"

class UProgressBar;
class AMonsterBase;

UCLASS()
class LSH_API UMonsterOffenserseBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterBase", meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monsterBase;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* offenserseBar;

public:
	void SetMonsterBase(AMonsterBase* _monsterBase);
};
