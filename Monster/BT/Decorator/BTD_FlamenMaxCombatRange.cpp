// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_FlamenMaxCombatRange.h"
#include "MonsterBaseController.h"
#include "MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterStateMachine.h"
#include "EAIState.h"

UBTD_FlamenMaxCombatRange::UBTD_FlamenMaxCombatRange()
{
    NodeName = TEXT("FlamenMaxCombatRange");
    maxCombatRange = 0.0f;
}

bool UBTD_FlamenMaxCombatRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    AMonsterBase* monsterBase = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if (!monsterBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("FlamenMaxCombatRange monsterBase null Error!"));
        return false;
    }

    float combatDistance = monsterBase->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (maxCombatRange < combatDistance)
    {
        monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::ReturnPatrol);
    }

    return true;
}
