// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterPointBar.generated.h"

class UProgressBar;
class UImage;
class AMonsterBase;
class UMonsterHealthBar;
class UMonsterCalHealthBar;
class UMonsterOffenserseBar;
class UMonsterCalOffenserseBar;

UCLASS(Abstract)
class LSH_API UMonsterPointBar : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MonsterBase", meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(meta = (BindWidget))
	UImage* monsterStateImage;

public:
	void SetMonsterBase(AMonsterBase* _monsterBase);

	FORCEINLINE UImage* GetMonsterStateImage()const { return monsterStateImage; }
};
