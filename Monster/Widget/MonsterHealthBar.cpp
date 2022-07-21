// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHealthBar.h"
#include "MonsterBase.h"


void UMonsterHealthBar::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
}
