// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"

UMonsterStateMachine::UMonsterStateMachine()
{
	PrimaryComponentTick.bCanEverTick = false;

	aiBehaviorState = EAI_Behavior_State::Idle;
	aiCombatState = EAI_Combat_State::Wait;
	aiMeleeAttackState = EAI_MeleeAttack_State::AttackGroup_1;
	aiAttackTypeState = EAI_AttackType_State::AttackWait;
	aiBossHealthState = EAI_BossHealth_State::State_1;
	aiBossWeaponTypeState = EAI_BossWeaponType_State::Katana;
}

void UMonsterStateMachine::BeginPlay()
{
	Super::BeginPlay();

	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);
}

void UMonsterStateMachine::SetBehaviorState(EAI_Behavior_State _state)
{
	aiBehaviorState = _state;
	uint8 castAIBehaviorState = static_cast<uint8>(aiBehaviorState);
	monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsEnum("enum_AIBehaviorState", castAIBehaviorState);
}

void UMonsterStateMachine::SetCombatState(EAI_Combat_State _state, bool _bOffenserseZeroEnd)
{
	if (aiBehaviorState == EAI_Behavior_State::Die)return;

	if(aiCombatState == EAI_Combat_State::OffenserseZero && !_bOffenserseZeroEnd)return;
	aiCombatState = _state;
	uint8 castAICombatState = static_cast<uint8>(aiCombatState);
	monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsEnum("enum_AICombatState", castAICombatState);
}

void UMonsterStateMachine::SetMeleeAttackState(EAI_MeleeAttack_State _state)
{
	aiMeleeAttackState = _state;
	uint8 castAIMeleeAttackState = static_cast<uint8>(aiMeleeAttackState);
	monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsEnum("enum_AIMeleeAttackState", castAIMeleeAttackState);
}

void UMonsterStateMachine::SetAttackTypeState(EAI_AttackType_State _state)
{
	aiAttackTypeState = _state;
}

void UMonsterStateMachine::SetMagicSpawnTypeState(EAI_MagicSpawnType_State _state)
{
	aiMagicSpawnTypeState = _state;
}

void UMonsterStateMachine::SetBossPhaseState(EAI_BossPhase_State _state)
{
	aiBossPhaseState = _state;
	uint8 castAIBossPhaseState = static_cast<uint8>(aiBossPhaseState);
	monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsEnum("enum_AIBossPhaseState", castAIBossPhaseState);
}

void UMonsterStateMachine::SetBossSpecialAttackState(EAI_BossSpecialAttack_State _state)
{
	aiBossSpecialAttackState = _state;
	uint8 castAIBossSpecialAttackState = static_cast<uint8>(aiBossSpecialAttackState);
	monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsEnum("enum_AIBossSpecialAttackState", castAIBossSpecialAttackState);
}

void UMonsterStateMachine::SetBossHealthState(EAI_BossHealth_State _state)
{
	aiBossHealthState = _state;
}

void UMonsterStateMachine::SetBossWeaponTypeState(EAI_BossWeaponType_State _state)
{
	aiBossWeaponTypeState = _state;
}

EAI_Behavior_State UMonsterStateMachine::GetBehaviorState() const
{
	EAI_Behavior_State behaviorState 
		= static_cast<EAI_Behavior_State>(monsterBase->MON_CTRL->GetBlackboardComponent()->GetValueAsEnum("enum_AIBehaviorState"));
	return behaviorState;
}

EAI_Combat_State UMonsterStateMachine::GetCombatState() const
{
	EAI_Combat_State combatState
		= static_cast<EAI_Combat_State>(monsterBase->MON_CTRL->GetBlackboardComponent()->GetValueAsEnum("enum_AICombatState"));
	return combatState;
}

EAI_MeleeAttack_State UMonsterStateMachine::GetMeleeAttackState() const
{
	EAI_MeleeAttack_State meleeAttackState
		= static_cast<EAI_MeleeAttack_State>(monsterBase->MON_CTRL->GetBlackboardComponent()->GetValueAsEnum("enum_AIMeleeAttackState"));
	return meleeAttackState;
}

EAI_AttackType_State UMonsterStateMachine::GetAttackTypeState() const
{
	return aiAttackTypeState;
}

EAI_MagicSpawnType_State UMonsterStateMachine::GetMagicSpawnTypeState() const
{
	return aiMagicSpawnTypeState;
}

EAI_BossPhase_State UMonsterStateMachine::GetBossPhaseState() const
{
	EAI_BossPhase_State bossPhaseState
		= static_cast<EAI_BossPhase_State>(monsterBase->MON_CTRL->GetBlackboardComponent()->GetValueAsEnum("enum_AIBossPhaseState"));
	return bossPhaseState;
}

EAI_BossSpecialAttack_State UMonsterStateMachine::GetBossSpecialAttackState() const
{
	EAI_BossSpecialAttack_State bossSpecialAttackState
		= static_cast<EAI_BossSpecialAttack_State>(monsterBase->MON_CTRL->GetBlackboardComponent()->GetValueAsEnum("enum_AIBossSpecialAttackState"));
	return bossSpecialAttackState;
}

EAI_BossHealth_State UMonsterStateMachine::GetBossHealthState() const
{
	return aiBossHealthState;
}

EAI_BossWeaponType_State UMonsterStateMachine::GetBossWeaponTypeState() const
{
	return aiBossWeaponTypeState;
}
