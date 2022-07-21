// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAI_Behavior_State : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	ReturnPatrol UMETA(DisplayName = "ReturnPatrol"),
	Investigate UMETA(DisplayName = "Investigate"),
	LookAround UMETA(DisplayName = "LookAround"),
	Battle UMETA(DisplayName = "Battle"),
	WeaponSwap UMETA(DisplayName = "WeaponSwap"),
	Die UMETA(DisplayName = "Die")
};

UENUM(BlueprintType)
enum class EAI_Combat_State : uint8
{
	Wait UMETA(DisplayName = "Wait"),
	Attack UMETA(DisplayName = "Attack"),
	DefenseStart UMETA(DisplayName = "DefenseStart"),
	SpecialSkill UMETA(DisplayName = "SpecialSkill"),
	DefenseEnd UMETA(DisplayName = "DefenseEnd"),
	DefenseSuccess UMETA(DisplayName = "DefenseSuccess"),
	Parry UMETA(DisplayName = "Parry"),
	AttackBlock UMETA(DisplayName = "AttackBlock"),
	Hit UMETA(DisplayName = "Hit"),
	OffenserseZero UMETA(DisplayName = "OffenserseZero"),
	Executed UMETA(DisplayName = "Executed"),
	Groggy UMETA(DisplayName = "Groggy"),
	OffenserseRecorvery UMETA(DisplayName = "OffenserseRecorvery"),
	Teleport UMETA(DisplayName = "Teleport"),
	InstantKill UMETA(DisplayName = "InstantKill")
};

UENUM(BlueprintType)
enum class EAI_MeleeAttack_State : uint8
{
	AttackGroup_1 UMETA(DisplayName = "AttackGroup_1"),
	AttackGroup_2 UMETA(DisplayName = "AttackGroup_2")
};

UENUM(BlueprintType)
enum class EAI_AttackType_State : uint8
{
	AttackWait UMETA(DisplayName = "AttackWait"),
	NormalTypeAttack UMETA(DisplayName = "NormalTypeAttack"),
	TakeDownTypeAttack UMETA(DisplayName = "TakeDownTypeAttack"),
	DotDamageTypeAttack UMETA(DisplayName = "DotDamageTypeAttack"),
	ArrowTypeAttack UMETA(DisplayName = "ArrowTypeAttack"),
	SpecialTypeAttack UMETA(DisplayName = "SpecialTypeAttack"),
	StunTypeAttack UMETA(DisplayName = "StunTypeAttack"),
	StunCheckTypeAttack UMETA(DisplayName = "StunCheckTypeAttack")
};

UENUM(BlueprintType)
enum class EAI_BossHealth_State : uint8
{
	State_1 UMETA(DisplayName = "State_1"),
	State_2 UMETA(DisplayName = "State_2"),
	State_3 UMETA(DisplayName = "State_3"),
	State_4 UMETA(DisplayName = "State_4")
};

UENUM(BlueprintType)
enum class EAI_MonsterType : uint8
{
	Warrior UMETA(DisplayName = "Warrior"),
	Sherden UMETA(DisplayName = "Sherden"),
	Guard UMETA(DisplayName = "Guard"),
	Flamen UMETA(DisplayName = "Flamen"),
	Boss UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class EAI_ElementalType_State : uint8
{
	Flame UMETA(DisplayName = "Flame"),
	Poison UMETA(DisplayName = "Poison"),
	Frozen UMETA(DisplayName = "Frozen"),
	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EAI_MagicSpawnType_State : uint8
{
	Default UMETA(DisplayName = "Default"),
	Projectile UMETA(DisplayName = "Projectile"),
	Orb UMETA(DisplayName = "Orb"),
	Pillar UMETA(DisplayName = "Pillar")
};

UENUM(BlueprintType)
enum class EAI_BossPhase_State : uint8
{
	FirstPhase UMETA(DisplayName = "FirstPhase"),
	SecondPhase UMETA(DisplayName = "SecondPhase"),
	PhaseLoading UMETA(DisplayName = "PhaseLoading")
};

UENUM(BlueprintType)
enum class EAI_BossWeaponType_State : uint8
{
	Katana UMETA(DisplayName = "Katana"),
	Mage UMETA(DisplayName = "Mage")
};

UENUM(BlueprintType)
enum class EAI_BossSpecialAttack_State : uint8
{
	AttackWait UMETA(DisplayName = "AttackWait"),
	SlashProjection UMETA(DisplayName = "SlashProjection")
};

class LSH_API EAIState
{
public:
	EAIState();
	~EAIState();
};
