// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Normal_WidgetComponent.h"
#include "MonsterPointBar.h"
#include "Components/Image.h"
#include "MonsterBase.h"
#include "Components/WidgetComponent.h"

UMonster_Normal_WidgetComponent::UMonster_Normal_WidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonster_Normal_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	monsterBase->GetMonsterPointBarWidget()->InitWidget();
	monsterPointBarHUD = Cast<UMonsterPointBar>(monsterBase->GetMonsterPointBarWidget()->GetUserWidgetObject());
	check(monsterPointBarHUD != nullptr);

	monsterPointBarHUD->SetMonsterBase(monsterBase);

	SetMonsterStateImageVisible(true);
	monsterBase->GetMonsterPointBarWidget()->SetVisibility(false);
}

void UMonster_Normal_WidgetComponent::SetMonsterStateImageVisible(bool _setHidden)
{
	Super::SetMonsterStateImageVisible(_setHidden);

	if (_setHidden)monsterPointBarHUD->GetMonsterStateImage()->SetVisibility(ESlateVisibility::Hidden);
	else monsterPointBarHUD->GetMonsterStateImage()->SetVisibility(ESlateVisibility::Visible);
}