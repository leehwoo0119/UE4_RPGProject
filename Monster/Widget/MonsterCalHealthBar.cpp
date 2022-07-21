// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCalHealthBar.h"
#include "MonsterBase.h"

void UMonsterCalHealthBar::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
}
