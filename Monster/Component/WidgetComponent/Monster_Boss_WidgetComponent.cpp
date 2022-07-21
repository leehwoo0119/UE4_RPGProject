// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Boss_WidgetComponent.h"
#include "MonsterBossPointBar.h"
#include "MonsterBase_Boss.h"

UMonster_Boss_WidgetComponent::UMonster_Boss_WidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonster_Boss_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMonster_Boss_WidgetComponent::SetMonsterBossPointBarHUD(UMonsterBossPointBar* _hud)
{
	Super::SetMonsterBossPointBarHUD(_hud);

	monsterBossPointBarHUD = _hud;
}

UMonsterBossPointBar* UMonster_Boss_WidgetComponent::GetMonsterBossPointBarHUD() const
{
	Super::GetMonsterBossPointBarHUD();
	return monsterBossPointBarHUD;
}

void UMonster_Boss_WidgetComponent::CallDrawBossPointBar(bool _draw)
{
	Super::CallDrawBossPointBar(_draw);
	DrawBossPointBar(_draw);
}