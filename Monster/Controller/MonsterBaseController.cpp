// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseAnimInstance.h"
#include "monsterCombatComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMonsterBaseController::AMonsterBaseController()
{

}

void AMonsterBaseController::BeginPlay()
{
	Super::BeginPlay();

	monsterBase = Cast<AMonsterBase>(GetPawn());
	check(monsterBase != nullptr);

}

void AMonsterBaseController::LogicStop()
{
	auto behaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	check(behaviorTreeComponent != nullptr);
	behaviorTreeComponent->StopTree(EBTStopMode::Safe);
}


