// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTimerComponent.h"

// Sets default values for this component's properties
UMonsterTimerComponent::UMonsterTimerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UMonsterTimerComponent::AllMonsterTimerClear()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(defenseEndTimer);
	GetOwner()->GetWorldTimerManager().ClearTimer(offenserseZeroTimer);
	GetOwner()->GetWorldTimerManager().ClearTimer(phaseChangeTimer_1);
	GetOwner()->GetWorldTimerManager().ClearTimer(phaseChangeTimer_2);
	GetOwner()->GetWorldTimerManager().ClearTimer(deathProssingTimer);
	GetOwner()->GetWorldTimerManager().ClearTimer(deathDestroyTimer);
	GetOwner()->GetWorldTimerManager().ClearTimer(setAttackEnableTimer);
}
