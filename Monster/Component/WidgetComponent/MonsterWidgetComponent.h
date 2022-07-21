// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterWidgetComponent.generated.h"

class AMonsterBase;
class UTargetLockOnWidget;
class UMonsterBossPointBar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSH_API UMonsterWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UTargetLockOnWidget* targetLockOnHUD;

protected:
	AMonsterBase* monsterBase;

public:
	FORCEINLINE UTargetLockOnWidget* GetTargetLockOnHUD()const { return targetLockOnHUD; }

	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterStateImageVisible(bool _setHidden = false) {}

	UFUNCTION(BlueprintCallable)
	virtual void SetMonsterBossPointBarHUD(UMonsterBossPointBar* _hud) {}

	UFUNCTION(BlueprintCallable)
	virtual UMonsterBossPointBar* GetMonsterBossPointBarHUD()const { return nullptr; }

	virtual void DrawBossPointBar(bool _draw) {}

	UFUNCTION(BlueprintCallable)
	virtual void CallDrawBossPointBar(bool _draw) {}
};
