// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECharState.generated.h"

UENUM(BlueprintType)
enum class EKatanaState_Type :uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Sprint UMETA(DisplayName = "Sprint"),
	Jump UMETA(DisplayName = "Jump"),
	Crouch UMETA(DisplayName = "Crouch"),
	SilentWalk UMETA(DisplayName = "SilentWalk"),
	MeleeAttackStart UMETA(DisplayName = "MeleeAttackStart"),
	MeleeAttack UMETA(DisplayName = "MeleeAttack"),
	Attacking UMETA(DisplayName = "Attacking"),
	CanNextMeleeAttack UMETA(DisplayName = "CanNextMeleeAttack"),
	Defense UMETA(DisplayName = "Defense"),
	Parrying UMETA(DisplayName = "Parrying"),
	Dash UMETA(DisplayName = "Dash"),
	CanDashRetry UMETA(DisplayName = "CanDashRetry"),
	Stealth UMETA(DisplayName = "Stealth"),
	Skill UMETA(DisplayName = "Skill"),
	WeaponSwap UMETA(DisplayName = "WeaponSwap"),
	CrouchWeaponSwap UMETA(DisplayName = "CrouchWeaponSwap"),
	TakeDamage UMETA(DisplayName = "TakeDamage"),
	TakeDownDamage UMETA(DisplayName = "TakeDownDamage"),
	ParryingSuccess UMETA(DisplayName = "ParryingSuccess"),
	CanParrying UMETA(DisplayName = "CanParrying"),
	Execution UMETA(DisplayName = "Execution"),
	DefenseBroken UMETA(DisplayName = "DefenseBroken"),
	TakeDownCanDash UMETA(DisplayName = "TakeDownCanDash"),
	HealthRecovery UMETA(DisplayName = "HealthRecovery"),
	Die UMETA(DisplayName = "Die"),
	DashInvincibility UMETA(DisplayName = "DashInvincibility"),
	AttackAvoid UMETA(DisplayName = "AttackAvoid"),
	Stun UMETA(DisplayName = "Stun"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EDashState_Type :uint8
{
	FrontDash UMETA(DisplayName = "FrontDash"),
	BackDash UMETA(DisplayName = "BackDash"),
	LeftDash UMETA(DisplayName = "LeftDash"),
	RightDash UMETA(DisplayName = "RightDash"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EAttackAvoidState_Type :uint8
{
	AttackAvoidFail UMETA(DisplayName = "AttackAvoidFail"),
	AttackAvoidSuccess UMETA(DisplayName = "AttackAvoidSuccess"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EStealthState_Type :uint8
{
	Stealth_1 UMETA(DisplayName = "Stealth_1"),
	Stealth_2 UMETA(DisplayName = "Stealth_2"),
	Stealth_3 UMETA(DisplayName = "Stealth_3"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EExecutionState_Type :uint8
{
	Execution_1 UMETA(DisplayName = "Execution_1"),
	Execution_2 UMETA(DisplayName = "Execution_2"),
	Execution_3 UMETA(DisplayName = "Execution_3"),
	Execution_4 UMETA(DisplayName = "Execution_4"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class ETargetSwitchState_Type :uint8
{
	TargetSwitchLeft UMETA(DisplayName = "TargetSwitchLeft"),
	TargetSwitchRight UMETA(DisplayName = "TargetSwitchRight"),
	NotSwitch UMETA(DisplayName = "NotSwitch"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EBloodDirectionState_Type :uint8
{
	Up UMETA(DisplayName = "Up"),
	RightUp UMETA(DisplayName = "RightUp"),
	Right UMETA(DisplayName = "Right"),
	RightDown UMETA(DisplayName = "RightDown"),
	Down UMETA(DisplayName = "Down"),
	LeftDown UMETA(DisplayName = "LeftDown"),
	Left UMETA(DisplayName = "Left"),
	LeftUp UMETA(DisplayName = "LeftUp"),
	Back UMETA(DisplayName = "Back"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EBowState_Type :uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Sprint UMETA(DisplayName = "Sprint"),
	Jump UMETA(DisplayName = "Jump"),
	Crouch UMETA(DisplayName = "Crouch"),
	SilentWalk UMETA(DisplayName = "SilentWalk"),
	Aiming UMETA(DisplayName = "Aiming"),
	FullCharging UMETA(DisplayName = "FullCharging"),
	Shooting UMETA(DisplayName = "Shooting"),
	WeaponSwap UMETA(DisplayName = "WeaponSwap"),
	CrouchWeaponSwap UMETA(DisplayName = "CrouchWeaponSwap"),
	TakeDamage UMETA(DisplayName = "TakeDamage"),
	TakeDownDamage UMETA(DisplayName = "TakeDownDamage"),
	TakeDownCanDash UMETA(DisplayName = "TakeDownCanDash"),
	HealthRecovery UMETA(DisplayName = "HealthRecovery"),
	Die UMETA(DisplayName = "Die"),
	Stun UMETA(DisplayName = "Stun"),

	Default UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EWeaponState_Type :uint8
{
	Katana UMETA(DisplayName = "Katana"),
	Unarmed UMETA(DisplayName = "Unarmed"),
	Bow UMETA(DisplayName = "Bow"),

	Default UMETA(DisplayName = "Default")
};