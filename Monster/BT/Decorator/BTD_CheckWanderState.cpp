// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckWanderState.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"

UBTD_CheckWanderState::UBTD_CheckWanderState()
{
	NodeName = TEXT("CheckWanderState");
}

bool UBTD_CheckWanderState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckWanderState aIMonster null Error!"));
		return false;
	}
	//if (aIMonster->GetMonsterStateMachine()->GetCombatState()==EAI_Combat_State::Groggy)return false;

	float monsterCharacterDis = aIMonster->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (monsterCharacterDis < aIMonster->MON_STATUS->GetChaseDis())
	{
		aIMonster->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		return true;
	}
	return false;
}