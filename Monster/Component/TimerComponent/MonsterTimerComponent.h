// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterTimerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSH_API UMonsterTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterTimerComponent();
		
public:
	FTimerHandle defenseEndTimer;
	FTimerHandle offenserseZeroTimer;
	FTimerHandle phaseChangeTimer_1;
	FTimerHandle phaseChangeTimer_2;
	FTimerHandle deathProssingTimer;
	FTimerHandle setAttackEnableTimer;
	FTimerHandle deathDestroyTimer;

public:
	void AllMonsterTimerClear();
};
