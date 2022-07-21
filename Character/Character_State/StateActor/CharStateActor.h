// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharStateActor.generated.h"

class UCharNormalState;
class UCharDashState;
class UCharCrouchState;
class UCharMeleeAttackState;
class UCharMeleeAttackStartState;
class UCharDefenseState;
class UCharParryingState;
class UCharStealthState;
class UCharUltimateCheckState;
class UCharUltimateState;
class UCharWeaponSwapState;
class UCharBowChargingState;
class UCharFullChargingState;
class UCharFullChargingFailState;
class UCharTakeDamageState;
class UCharTakeDownDamageState;
class UCharDefenseSuccessState;
class UCharParryingSuccessState;
class UCharDefenseBrokenState;
class UCharExecutionState;
class UCharRecoveryState;
class UCharDieState;
class UCharCheckAttackAvoidState;
class UCharAttackAvoidState;
class UCharInstantDeathState;
class UCharStunState;

UCLASS()
class LHW_API ACharStateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharStateActor();

protected:
	// Called when the game starts
	void BeginPlay() override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharNormalState* charNormalState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharDashState* charDashState;	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharCrouchState* charCrouchState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharMeleeAttackState* charMeleeAttackState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharMeleeAttackStartState* charMeleeAttackStartState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharDefenseState* charDefenseState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharParryingState* charParryingState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharStealthState* charStealthState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharUltimateState* charUltimateState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharWeaponSwapState* charWeaponSwapState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharBowChargingState* charBowChargingState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharFullChargingState* charFullChargingState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharFullChargingFailState* charFullChargingFailState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharTakeDamageState* charTakeDamageState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharTakeDownDamageState* charTakeDownDamageState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharDefenseSuccessState* charDefenseSuccessState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharParryingSuccessState* charParryingSuccessState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharDefenseBrokenState* charDefenseBrokenState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharExecutionState* charExecutionState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharRecoveryState* charRecoveryState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharDieState* charDieState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharCheckAttackAvoidState* charCheckAttackAvoidState;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharAttackAvoidState* charAttackAvoidState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharInstantDeathState* charInstantDeathState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharState")
	UCharStunState* charStunState;

public:
	UFUNCTION(BlueprintCallable)
	void SetCharExecutionState(UCharExecutionState* _state);

	UFUNCTION(BlueprintCallable)
	void SetCharStealthState(UCharStealthState* _state);
};
