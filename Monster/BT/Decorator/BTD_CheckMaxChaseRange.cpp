// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckMaxChaseRange.h"
#include "LHW/PatrolPathInterface.h"
#include "LHW/PatrolPath.h"
#include "LHW/MainChar.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBaseStatus.h"

UBTD_CheckMaxChaseRange::UBTD_CheckMaxChaseRange()
{
	NodeName = TEXT("CheckMaxChaseRange");
	maxChaseRange = 0.0f;
}

bool UBTD_CheckMaxChaseRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* monsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!monsterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckMaxChaseRange monsterBase null Error!"));
		return false;
	}
	
	AMonsterBaseController* monsterBaseController = Cast<AMonsterBaseController>(OwnerComp.GetAIOwner());
	if (!monsterBaseController)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckMaxChaseRange monsterBaseController null Error!"));
		return false;
	}
	
	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
	if (!monsterBaseController)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckMaxChaseRange myBlackboard null Error!"));
		return false;
	}

	IPatrolPathInterface* patrolPath = Cast<IPatrolPathInterface>(monsterBase);
	if (patrolPath->GetPatrolPath())
		myBlackboard->SetValueAsVector("returnLocationKey", patrolPath->GetPatrolPath()->GetNextPointWorldPos());
	else
		myBlackboard->SetValueAsVector("returnLocationKey", monsterBase->GetStartLocation());

	float distance = 
		FVector::Distance(
			monsterBase->GetActorLocation(),
			myBlackboard->GetValueAsVector("returnLocationKey"));
	//monsterBase->MON_STATUS->GetPlayerSee()
	if (distance >= maxChaseRange &&
			monsterBase->MON_COMBATSTATE != EAI_Combat_State::Attack &&
			monsterBase->MON_COMBATSTATE != EAI_Combat_State::SpecialSkill)
		
	{
		if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Battle)
		{
			monsterBase->MON_ANIM->AnimMontagePlay(
				monsterBase->MON_ANIM->weaponSwapMontage,
				1.4f,
				FName("Unarm"));
		}		
		monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::ReturnPatrol);
	}
	
	return true;
}