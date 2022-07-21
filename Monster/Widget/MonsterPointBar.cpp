// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterPointBar.h"
#include "MonsterHealthBar.h"
#include "MonsterCalHealthBar.h"
#include "MonsterOffenserseBar.h"
#include "MonsterCalOffenserseBar.h"
#include "MonsterBase.h"

void UMonsterPointBar::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
	MonsterHealthBar->SetMonsterBase(monsterBase);
	MonsterCalHealthBar->SetMonsterBase(monsterBase);
	MonsterOffenserseBar->SetMonsterBase(monsterBase);
	MonsterCalOffenserseBar->SetMonsterBase(monsterBase);
}
