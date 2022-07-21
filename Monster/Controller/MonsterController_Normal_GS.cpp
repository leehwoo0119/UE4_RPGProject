// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Normal_GS.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterCombatComponent.h"
#include "MonsterBaseStatus.h"

AMonsterController_Normal_GS::AMonsterController_Normal_GS()
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

void AMonsterController_Normal_GS::BeginPlay()
{
	Super::BeginPlay();

}

void AMonsterController_Normal_GS::PlayerAttackDetected()
{
	Super::PlayerAttackDetected();

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Battle)
	{
		if (monsterBase->MON_COMBATSTATE != EAI_Combat_State::Attack &&
			monsterBase->MON_COMBATSTATE != EAI_Combat_State::Hit)
		{
			if (!SpecialSkill())return;
			monsterBase->GetMonsterCombatComponent()->SpecialSkill();
		}

	}

}

bool AMonsterController_Normal_GS::SpecialSkill()
{
	Super::SpecialSkill();

	if (monsterBase->MON_STATUS->GetDefenseCnt() == 0)
	{
		int randNum = FMath::RandRange(1, 100);
		//UE_LOG(LogTemp, Log, TEXT("%d"), randNum);
		if (randNum < monsterBase->MON_STATUS->GetCurMonsterOffensersePoint())
		{
			int randDefenseCnt = FMath::RandRange(1, 3);
			//UE_LOG(LogTemp, Log, TEXT("%f"),  monsterBase->GetMonsterStatus()->GetCurMonsterOffensersePoint());
			monsterBase->MON_STATUS->SetDefenseCnt(randDefenseCnt);
			return true;
		}
	}
	return false;
}