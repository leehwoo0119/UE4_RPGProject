// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Normal_SF.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"

AMonsterController_Normal_SF::AMonsterController_Normal_SF()
{
	aiSightRadius = 2000.0f;
	aiLoseSightRadius = 3000.0f;
	aiFieldOfView = 90.0f;
	aiViewBackwardOffset = 100.0f;
	aiNearClippingRadius = 100.0f;
	aiHearingRange = 2000.0f;

	PerceptionSetting();

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/LSH/BB_BT/BB_Monster_Normal.BB_Monster_Normal'"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/LSH/BB_BT/BT_Flamen.BT_Flamen'"));
	check(BBObject.Succeeded());
	check(BTObject.Succeeded());

	BBAsset = BBObject.Object;
	BTAsset = BTObject.Object;
}

void AMonsterController_Normal_SF::BeginPlay()
{
	Super::BeginPlay();

}
