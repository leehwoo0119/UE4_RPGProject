// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.generated.h"

class AMonsterBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSH_API UMonsterStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	UMonsterStateMachine();

protected:
	virtual void BeginPlay() override;

private:
	AMonsterBase* monsterBase;

	EAI_Behavior_State aiBehaviorState;
	EAI_Combat_State aiCombatState;
	EAI_MeleeAttack_State aiMeleeAttackState;
	EAI_AttackType_State aiAttackTypeState;
	EAI_MagicSpawnType_State aiMagicSpawnTypeState;
	EAI_BossPhase_State aiBossPhaseState;
	EAI_BossSpecialAttack_State aiBossSpecialAttackState;
	EAI_BossHealth_State aiBossHealthState;
	EAI_BossWeaponType_State aiBossWeaponTypeState;

public:
	UFUNCTION(BlueprintCallable)
	void SetBehaviorState(EAI_Behavior_State _state);

	UFUNCTION(BlueprintCallable)
	void SetCombatState(EAI_Combat_State _state, bool _bOffenserseZeroEnd = false);

	UFUNCTION(BlueprintCallable)
	void SetMeleeAttackState(EAI_MeleeAttack_State _state);

	UFUNCTION(BlueprintCallable)
	void SetAttackTypeState(EAI_AttackType_State _state);

	UFUNCTION(BlueprintCallable)
	void SetMagicSpawnTypeState(EAI_MagicSpawnType_State _state);

	UFUNCTION(BlueprintCallable)
	void SetBossPhaseState(EAI_BossPhase_State _state);

	UFUNCTION(BlueprintCallable)
	void SetBossSpecialAttackState(EAI_BossSpecialAttack_State _state);

	void SetBossHealthState(EAI_BossHealth_State _state);

	void SetBossWeaponTypeState(EAI_BossWeaponType_State _state);

	UFUNCTION(BlueprintCallable)
	EAI_Behavior_State GetBehaviorState()const;

	UFUNCTION(BlueprintCallable)
	EAI_Combat_State GetCombatState()const;

	UFUNCTION(BlueprintCallable)
	EAI_MeleeAttack_State GetMeleeAttackState()const;

	UFUNCTION(BlueprintCallable)
	EAI_AttackType_State GetAttackTypeState()const;

	EAI_MagicSpawnType_State GetMagicSpawnTypeState()const;

	UFUNCTION(BlueprintCallable)
	EAI_BossPhase_State GetBossPhaseState()const;

	UFUNCTION(BlueprintCallable)
	EAI_BossSpecialAttack_State GetBossSpecialAttackState()const;

	EAI_BossHealth_State GetBossHealthState()const;

	UFUNCTION(BlueprintCallable)
	EAI_BossWeaponType_State GetBossWeaponTypeState()const;
};
