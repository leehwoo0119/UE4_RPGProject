// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossStatus.h"

UMonsterBossStatus::UMonsterBossStatus()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterBossStatus::BeginPlay()
{
	Super::BeginPlay();

	currentGroggyCount = 0;
	maxGroggyCount = 50;
	maxWhirlwindCnt = 0;
	whirlwindCnt = 0;
	maxMageAttackCnt = 3;
	maxOffenserseRecoveryAttackedCnt = 3;
	offenserseRecoveryAttackedCnt = 0;

	bCheckOffenserseRecorvery = true;	
	bIsEnableHealthDrain = false;
	bCanAvoidAttack = false;

	for (int i = 0; i < 3; i++)
	{
		bSpecialAttackArr.Add(false);
	}
	
}

void UMonsterBossStatus::SetCurrentGroggyCount(int _value)
{
	Super::SetCurrentGroggyCount(_value);

	currentGroggyCount = _value;
}

void UMonsterBossStatus::SetMaxGroggyCount(int _value)
{
	Super::SetMaxGroggyCount(_value);

	maxGroggyCount = _value;
}

void UMonsterBossStatus::PlusCurrentGroggyCount(int _value)
{
	Super::PlusCurrentGroggyCount(_value);

	currentGroggyCount += _value;
}

void UMonsterBossStatus::PlusMageAttackCnt(bool _reset)
{
	if (_reset)
	{
		mageAttackCnt = 0;
	}
	else
	{
		mageAttackCnt++;
	}
}

void UMonsterBossStatus::SetMaxAttackCount(int _value)
{
	Super::SetMaxAttackCount(_value);

	maxAttackCount = _value;
}

void UMonsterBossStatus::SetCurrentProjectileCount(int _value)
{
	Super::SetCurrentProjectileCount(_value);

	currentProjectileCount = _value;
}

void UMonsterBossStatus::SetMaxProjectileCount(int _value)
{
	Super::SetMaxProjectileCount(_value);

	maxProjectileCount = _value;
}

void UMonsterBossStatus::PlusCurrentProjectileCount()
{
	Super::PlusCurrentProjectileCount();

	currentProjectileCount++;
}

void UMonsterBossStatus::SetCheckOffenserseRecorvery(bool _value)
{
	Super::SetCheckOffenserseRecorvery(_value);

	bCheckOffenserseRecorvery = _value;
}

void UMonsterBossStatus::SetTrueSpecialAttack(int _index)
{
	Super::SetTrueSpecialAttack(_index);

	bSpecialAttackArr[_index] = true;
}

void UMonsterBossStatus::SetEnableHealthDrain(bool _value)
{
	Super::SetEnableHealthDrain(_value);

	bIsEnableHealthDrain = _value;
}

void UMonsterBossStatus::SetCanAvoidAttack(bool _value)
{
	bCanAvoidAttack = _value;
}

void UMonsterBossStatus::SetMaxWhirlwindCnt(int _value)
{
	Super::SetMaxWhirlwindCnt(_value);

	maxWhirlwindCnt = _value;
}

void UMonsterBossStatus::PlusOrResetWhirlwindCnt(bool _reset)
{
	Super::PlusOrResetWhirlwindCnt(_reset);

	if (_reset)
	{
		whirlwindCnt = 0;
	}
	else
	{
		whirlwindCnt++;
	}
}

bool UMonsterBossStatus::CheckOffenserseRecovery()
{
	Super::CheckOffenserseRecovery();

	offenserseRecoveryAttackedCnt++;
	if (maxOffenserseRecoveryAttackedCnt <= offenserseRecoveryAttackedCnt)
	{
		offenserseRecoveryAttackedCnt = 0;
		return true;
	}
	return false;
}
