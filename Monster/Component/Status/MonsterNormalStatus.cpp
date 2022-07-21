// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalStatus.h"

UMonsterNormalStatus::UMonsterNormalStatus()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	defenseCnt = 0;
	bInvestigateMoveStart = false;
}

// Called when the game starts
void UMonsterNormalStatus::BeginPlay()
{
	Super::BeginPlay();

}

void UMonsterNormalStatus::SetDefenseCnt(int _cnt)
{
	Super::SetDefenseCnt(_cnt);

	defenseCnt = _cnt;
}

void UMonsterNormalStatus::SetInvestigateMoveStart(bool _val)
{
	Super::SetInvestigateMoveStart(_val);

	bInvestigateMoveStart = _val;
}

void UMonsterNormalStatus::SetElementalType(EAI_ElementalType_State _elemental)
{
	Super::SetElementalType(_elemental);

	aiElementalTypeState = _elemental;
}
