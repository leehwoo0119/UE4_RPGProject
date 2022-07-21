// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCalOffenserseBar.h"
#include "MonsterBase.h"

void UMonsterCalOffenserseBar::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
}
