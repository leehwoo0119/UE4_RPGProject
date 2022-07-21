// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"


namespace blackboard_keys
{
	TCHAR const* const targetLocation = TEXT("targetLocationKey");
	TCHAR const* const enum_AIBehaviorState = TEXT("enum_AIBehaviorState");
	TCHAR const* const focusActor = TEXT("focusActorKey");
	TCHAR const* const enum_AICombatState = TEXT("enum_AICombatState");
	TCHAR const* const enum_AIMeleeAttackState = TEXT("enum_AIMeleeAttackState");
	TCHAR const* const returnLocation = TEXT("returnLocationKey");
	TCHAR const* const bIsCanSeePlayer = TEXT("bIsCanSeePlayer");
	TCHAR const* const enum_AIBossPhaseState = TEXT("enum_AIBossPhaseState");
	TCHAR const* const enum_AIBossSpecialAttackState = TEXT("enum_AIBossSpecialAttackState");
}

class LSH_API BlackBoard_Monster_Key
{
public:
	BlackBoard_Monster_Key();
	~BlackBoard_Monster_Key();
};
