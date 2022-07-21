// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterWidgetComponent.h"
#include "Monster_Boss_WidgetComponent.generated.h"

class UMonsterBossPointBar;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSH_API UMonster_Boss_WidgetComponent : public UMonsterWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UMonster_Boss_WidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UMonsterBossPointBar* monsterBossPointBarHUD;

public:
	void SetMonsterBossPointBarHUD(UMonsterBossPointBar* _hud)override;

	UMonsterBossPointBar* GetMonsterBossPointBarHUD()const override;

	UFUNCTION(BlueprintImplementableEvent)
	void DrawBossPointBar(bool _draw)override;

	void CallDrawBossPointBar(bool _draw)override;
};
