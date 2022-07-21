// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidgetComponent.h"
#include "TargetLockOnWidget.h"
#include "Components/WidgetComponent.h"
#include "MonsterBase.h"
#include "MonsterCombatComponent.h"

// Sets default values for this component's properties
UMonsterWidgetComponent::UMonsterWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
}


// Called when the game starts
void UMonsterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);
	
	targetLockOnHUD = Cast<UTargetLockOnWidget>(monsterBase->GetTargetLockOnWidget()->GetUserWidgetObject());
	check(targetLockOnHUD != nullptr);
	
	targetLockOnHUD->SetOffenserseZero(false);
	monsterBase->GetTargetLockOnWidget()->SetVisibility(false);
}
