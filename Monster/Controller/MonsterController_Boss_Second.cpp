// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Boss_Second.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"

AMonsterController_Boss_Second::AMonsterController_Boss_Second()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/LSH/BB_BT/BB_Monster_Normal.BB_Monster_Normal'"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/LSH/BB_BT/BT_SecondBoss.BT_SecondBoss'"));
	check(BBObject.Succeeded());
	check(BTObject.Succeeded());

	BBAsset = BBObject.Object;
	BTAsset = BTObject.Object;
}

void AMonsterController_Boss_Second::BeginPlay()
{
	Super::BeginPlay();

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}

	GetBlackboardComponent()->SetValueAsEnum("enum_AIBehaviorState", (uint8)EAI_Behavior_State::Idle);
	GetBlackboardComponent()->SetValueAsEnum("enum_AICombatState", (uint8)EAI_Combat_State::Wait);
	GetBlackboardComponent()->SetValueAsEnum("enum_AIMeleeAttackState", (uint8)EAI_MeleeAttack_State::AttackGroup_1);
	GetBlackboardComponent()->SetValueAsEnum("enum_AIBossPhaseState", (uint8)EAI_BossPhase_State::FirstPhase);
	GetBlackboardComponent()->SetValueAsEnum("enum_AIBossSpecialAttackState", (uint8)EAI_BossSpecialAttack_State::AttackWait);
}
