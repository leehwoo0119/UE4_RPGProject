// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStateActor.h"
#include "CharNormalState.h"
#include "CharDashState.h"
#include "CharCrouchState.h"
#include "CharMeleeAttackState.h"
#include "CharMeleeAttackStartState.h"
#include "CharDefenseState.h"
#include "CharParryingState.h"
#include "CharUltimateState.h"
#include "CharStealthState.h"
#include "CharWeaponSwapState.h"
#include "CharBowChargingState.h"
#include "CharFullChargingState.h"
#include "CharFullChargingFailState.h"
#include "CharTakeDamageState.h"
#include "CharTakeDownDamageState.h"
#include "CharDefenseSuccessState.h"
#include "CharParryingSuccessState.h"
#include "CharDefenseBrokenState.h"
#include "CharExecutionState.h"
#include "CharRecoveryState.h"
#include "CharDieState.h"
#include "CharCheckAttackAvoidState.h"
#include "CharAttackAvoidState.h"
#include "CharInstantDeathState.h"
#include "CharStunState.h"

// Sets default values
ACharStateActor::ACharStateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	charNormalState = CreateDefaultSubobject<UCharNormalState>(FName("charNormalState"));
	charDashState = CreateDefaultSubobject<UCharDashState>(FName("charDashState"));	
	charCrouchState = CreateDefaultSubobject<UCharCrouchState>(FName("charCrouchState"));
	charTakeDamageState = CreateDefaultSubobject<UCharTakeDamageState>(FName("charTakeDamageState"));
	charTakeDownDamageState = CreateDefaultSubobject<UCharTakeDownDamageState>(FName("charTakeDownDamageState"));

	charMeleeAttackState = CreateDefaultSubobject<UCharMeleeAttackState>(FName("charMeleeAttackState"));
	charMeleeAttackStartState = CreateDefaultSubobject<UCharMeleeAttackStartState>(FName("charMeleeAttackStartState"));
	charDefenseState = CreateDefaultSubobject<UCharDefenseState>(FName("charDefenseState"));
	charParryingState = CreateDefaultSubobject<UCharParryingState>(FName("charParryingState"));
	//charStealthState = CreateDefaultSubobject<UCharStealthState>(FName("charStealthState"));
	charUltimateState = CreateDefaultSubobject<UCharUltimateState>(FName("charUltimateState"));
	charWeaponSwapState = CreateDefaultSubobject<UCharWeaponSwapState>(FName("charWeaponSwapState"));
	charDefenseSuccessState = CreateDefaultSubobject<UCharDefenseSuccessState>(FName("charDefenseSuccessState"));
	charParryingSuccessState = CreateDefaultSubobject<UCharParryingSuccessState>(FName("charParryingSuccessState"));
	charDefenseBrokenState = CreateDefaultSubobject<UCharDefenseBrokenState>(FName("charDefenseBrokenState"));
	//charExecutionState = CreateDefaultSubobject<UCharExecutionState>(FName("charExecutionState"));
	charRecoveryState = CreateDefaultSubobject<UCharRecoveryState>(FName("charRecoveryState"));
	charDieState = CreateDefaultSubobject<UCharDieState>(FName("charDieState"));
	charCheckAttackAvoidState = CreateDefaultSubobject<UCharCheckAttackAvoidState>(FName("charCheckAttackAvoidState"));
	charAttackAvoidState = CreateDefaultSubobject<UCharAttackAvoidState>(FName("charAttackAvoidState"));
	charInstantDeathState = CreateDefaultSubobject<UCharInstantDeathState>(FName("charInstantDeathState"));
	charStunState = CreateDefaultSubobject<UCharStunState>(FName("charStunState"));
		
	charBowChargingState = CreateDefaultSubobject<UCharBowChargingState>(FName("charBowChargingState"));
	charFullChargingState = CreateDefaultSubobject<UCharFullChargingState>(FName("charFullChargingState"));
	charFullChargingFailState = CreateDefaultSubobject<UCharFullChargingFailState>(FName("charFullChargingFailState"));

}

void ACharStateActor::BeginPlay()
{
	Super::BeginPlay();

	check(charNormalState != nullptr);
	check(charDashState != nullptr);
	check(charCrouchState != nullptr);
	check(charTakeDamageState != nullptr); 
	check(charTakeDownDamageState != nullptr);

	check(charMeleeAttackState != nullptr);
	check(charMeleeAttackStartState != nullptr);
	check(charDefenseState != nullptr);
	check(charParryingState != nullptr);
	//check(charStealthState != nullptr);
	check(charUltimateState != nullptr);
	check(charWeaponSwapState != nullptr);
	check(charDefenseSuccessState != nullptr);
	check(charParryingSuccessState != nullptr);
	check(charDefenseBrokenState != nullptr);
	//check(charExecutionState != nullptr);
	check(charRecoveryState != nullptr);
	check(charDieState != nullptr);
	check(charCheckAttackAvoidState != nullptr);
	check(charAttackAvoidState != nullptr);
	check(charInstantDeathState != nullptr);
	check(charStunState != nullptr);

	check(charBowChargingState != nullptr);
	check(charFullChargingState != nullptr);
	check(charFullChargingFailState != nullptr);
}

void ACharStateActor::SetCharExecutionState(UCharExecutionState* _state)
{
	charExecutionState = _state;
	check(charExecutionState != nullptr);
}

void ACharStateActor::SetCharStealthState(UCharStealthState* _state)
{
	charStealthState = _state;
	check(charStealthState != nullptr);
}
