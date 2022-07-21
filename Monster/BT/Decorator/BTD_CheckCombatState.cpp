// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckCombatState.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"

UBTD_CheckCombatState::UBTD_CheckCombatState()
{
	NodeName = TEXT("CheckCombatState");
}

bool UBTD_CheckCombatState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckCombatState aIMonster null Error!"));
		return false;
	}

	float monsterCharacterDis = aIMonster->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (monsterCharacterDis < aIMonster->MON_STATUS->GetChaseDis() - 200.0f)
	{
		aIMonster->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		return true;
	}
	return false;
}