// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterWidgetComponent.h"
#include "Monster_Normal_WidgetComponent.generated.h"

class UMonsterPointBar;

UCLASS()
class LSH_API UMonster_Normal_WidgetComponent : public UMonsterWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UMonster_Normal_WidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UMonsterPointBar* monsterPointBarHUD;

public:
	FORCEINLINE UMonsterPointBar* GetMonsterPointBarHUD()const { return monsterPointBarHUD; }

	void SetMonsterStateImageVisible(bool _setHidden = false)override;
};
