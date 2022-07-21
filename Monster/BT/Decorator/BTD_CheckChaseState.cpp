// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckChaseState.h"
#include "MonsterBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"

UBTD_CheckChaseState::UBTD_CheckChaseState()
{
	NodeName = TEXT("CheckChaseState");
}

bool UBTD_CheckChaseState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	AMonsterBase* aIMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckChaseState aIMonster null Error!"));
		return false;
	}

	float monsterCharacterDis = aIMonster->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (monsterCharacterDis >= aIMonster->MON_STATUS->GetChaseDis())
	{
		aIMonster->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		return true;
	}
	return false;
}