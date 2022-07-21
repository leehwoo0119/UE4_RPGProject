// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Normal_KN.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterCombatComponent.h"
#include "MonsterBaseStatus.h"

AMonsterController_Normal_KN::AMonsterController_Normal_KN()
{
	aiSightRadius = 1000.0f;
	aiLoseSightRadius = 1700.0f;
	aiFieldOfView = 60.0f;
	aiViewBackwardOffset = 100.0f;
	aiNearClippingRadius = 100.0f;
	aiHearingRange = 1000.0f;

	PerceptionSetting();

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/LSH/BB_BT/BB_Monster_Normal.BB_Monster_Normal'"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/LSH/BB_BT/BT_Melee.BT_Melee'"));
	check(BBObject.Succeeded());
	check(BTObject.Succeeded());

	BBAsset = BBObject.Object;
	BTAsset = BTObject.Object;
}

void AMonsterController_Normal_KN::BeginPlay()
{
	Super::BeginPlay();

}

void AMonsterController_Normal_KN::PlayerAttackDetected()
{
	Super::PlayerAttackDetected();

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Battle)
	{
		if (monsterBase->MON_COMBATSTATE != EAI_Combat_State::Attack &&
			monsterBase->MON_COMBATSTATE != EAI_Combat_State::SpecialSkill &&
			monsterBase->MON_COMBATSTATE != EAI_Combat_State::Hit)
		{
			if (!SpecialSkill())return;
			monsterBase->GetMonsterCombatComponent()->SpecialSkill();
		}

	}

}

bool AMonsterController_Normal_KN::SpecialSkill()
{
	Super::SpecialSkill();

	int randNum = FMath::RandRange(1, 100);
	//UE_LOG(LogTemp, Log, TEXT("%d"), randNum);
	if (randNum < monsterBase->MON_STATUS->GetCurMonsterOffensersePoint())
	{
		return true;
	}
	return false;
}
