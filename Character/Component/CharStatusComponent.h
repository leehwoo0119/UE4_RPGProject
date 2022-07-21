// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECharState.h"
#include "Components/TimeLineComponent.h"
#include "CharStatusComponent.generated.h"

class ACanTargetingActor;
class AMonsterBase_Normal;
class AMainChar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LHW_API UCharStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EKatanaState_Type katanaState;
	EKatanaState_Type prevKatanaState;
	EBowState_Type bowState;
	EBowState_Type prevBowState;
	EDashState_Type dashState;
	EStealthState_Type stealthState;
	EExecutionState_Type executionState;
	EWeaponState_Type weaponTypeState;
	EWeaponState_Type prevWeaponTypeState;
	EBloodDirectionState_Type bloodTypeDirectionState;
	EAttackAvoidState_Type attackAvoidState;
	ETargetSwitchState_Type targetSwitchState;

	AMonsterBase_Normal* stealthTarget;
	AMonsterBase* executionTarget;
	AMonsterBase* attackAvoidTarget;
	uint8 meleeAttackCnt;

	bool bTargetLockOn;
	bool bCanStealth;
	bool bDefense;
	bool bMoveStop;
	bool bStaminaPointRecovery;
	bool bInvincibility;

	float damageDirection;
	float damage;

	float maxCharHealthPoint;
	float maxCharStaminaPoint;
	float maxCharRecoveryPoint;
	float maxCharUltimatePoint;

	float behaviorStaminaCost;

	float calCharHealthPoint;
	float calCharStaminaPoint;

	float curCharHealthPoint;
	float curCharStaminaPoint;
	float curCharRecoveryPoint;
	float curCharUltimatePoint;

	float recoveryStaminaValue;

	int stunCnt;

	bool bCanUseUltimate;

	TArray<ACanTargetingActor*> attackedMonsterArr;

	AMainChar* mainChar;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* healthDecreaseCurveFloat;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* staminaDecreaseCurveFloat;

	FTimeline healthDecreaseCurveFTimeline;
	FTimeline staminaDecreaseCurveFTimeline;

	float healthDecreaseCurveLength;
	float staminaDecreaseCurveLength;

	FTimerHandle calStaminaPointRecoveryTimer;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EKatanaState_Type GetKatanaState()const { return katanaState; }

	FORCEINLINE EKatanaState_Type GetPrevKatanaState()const { return prevKatanaState; }

	FORCEINLINE EBowState_Type GetBowState()const { return bowState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EBowState_Type GetPrevBowState()const { return prevBowState; }

	FORCEINLINE EDashState_Type GetDashState()const { return dashState; }
	FORCEINLINE EStealthState_Type GetStealthState()const { return stealthState; }
	FORCEINLINE EExecutionState_Type GetExecutionState()const { return executionState; }
	FORCEINLINE EWeaponState_Type GetWeaponState()const { return weaponTypeState; }
	FORCEINLINE EWeaponState_Type GetPrevWeaponState()const { return prevWeaponTypeState; }
	FORCEINLINE EBloodDirectionState_Type GetBloodDirectionState()const { return bloodTypeDirectionState; }
	FORCEINLINE EAttackAvoidState_Type GetAttackAvoid()const { return attackAvoidState; }
	FORCEINLINE ETargetSwitchState_Type GetTargetSwitchState()const { return targetSwitchState; }
	
	FORCEINLINE AMonsterBase_Normal* GetStealthTarget()const { return stealthTarget; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMonsterBase* GetExecutionTarget()const { return executionTarget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMonsterBase* GetAttackAvoidTarget()const { return attackAvoidTarget; }

	UFUNCTION(BlueprintCallable)
	uint8 GetMeleeAttackCnt()const { return meleeAttackCnt; }

	FORCEINLINE bool GetTargetLockOn()const { return bTargetLockOn; }
	FORCEINLINE bool GetCanStealth()const { return bCanStealth; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetDefense()const { return bDefense; }

	FORCEINLINE bool GetMoveStop()const { return bMoveStop; }
	FORCEINLINE bool GetStaminaPointRecovery()const { return bStaminaPointRecovery; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetInvincibility()const { return bInvincibility; }

	FORCEINLINE float GetDamageDirection()const { return damageDirection; }
	FORCEINLINE float GetDamage()const { return damage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxCharHealthPoint()const { return maxCharHealthPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCalCharHealthPoint()const { return calCharHealthPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurCharHealthPoint()const { return curCharHealthPoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxCharStaminaPoint()const { return maxCharStaminaPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCalCharStaminaPoint()const { return calCharStaminaPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurCharStaminaPoint()const { return curCharStaminaPoint; }
	FORCEINLINE float GetRecoveryStaminaValue()const { return recoveryStaminaValue; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxCharRecoveryPoint()const { return maxCharRecoveryPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurCharRecoveryPoint()const { return curCharRecoveryPoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxCharUltimatePoint()const { return maxCharUltimatePoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurCharUltimatePoint()const { return curCharUltimatePoint; }

	FORCEINLINE float GetBehaviorStaminaCost()const { return behaviorStaminaCost; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetStunCnt()const { return stunCnt; }

	FORCEINLINE bool GetCanUseUltimate()const { return bCanUseUltimate; }

	FORCEINLINE TArray<ACanTargetingActor*> GetAttackedMonsterArr()const { return attackedMonsterArr; }

	void SetCalCharHealthPoint(float _val);

	UFUNCTION(BlueprintCallable)
	void SetKatanaState(EKatanaState_Type _state);

	UFUNCTION(BlueprintCallable)
	void SetBowState(EBowState_Type _state);

	void SetDashState(EDashState_Type _state);

	void SetStealthState(EStealthState_Type _state);

	void SetExecutionState(EExecutionState_Type _state);

	UFUNCTION(BlueprintCallable)
	void SetBloodTypeDirectionState(EBloodDirectionState_Type _state);

	void SetAttackAvoid(EAttackAvoidState_Type _val);

	void SetTargetSwitchState(ETargetSwitchState_Type _val);

	void SetDamageDirection(float _val);

	void SetDamage(float _val);

	UFUNCTION(BlueprintCallable)
	bool PlusOrSetCurCharHealthPoint(float _val, bool _bSetVal = false);					

	UFUNCTION(BlueprintCallable)
	void PlusOrSetCurCharStaminaPoint(float _val, bool _bSetVal = false);

	UFUNCTION(BlueprintCallable)
	void PlusOrSetCurCharRecoveryPoint(float _val, bool _bSetVal = false);

	UFUNCTION(BlueprintCallable)
	void PlusOrSetCurCharUltimatePoint(float _val, bool _bSetVal = false);

	UFUNCTION(BlueprintCallable)
	void SetWeaponState(EWeaponState_Type _state);

	void SetStealthTarget(AMonsterBase_Normal* _target);

	void SetExecutionTarget(AMonsterBase* _target);

	void SetAttackAvoidTarget(AMonsterBase* _target);

	void SetTargetLockOn(bool _val);

	void SetCanStealth(bool _val);

	void SetDefense(bool _val);

	void SetMoveStop(bool _val);

	void SetStaminaPointRecovery(bool _val);

	void SetInvincibility(bool _val);

	void PlusMeleeAttackCnt();

	void ResetMeleeAttack();

	bool CheckCanUseStaminaBehavior();

	bool CheckCanUseRecoveryBehavior();

	void AddAttackedMonsterArr(ACanTargetingActor* _target);

	void DeleteAttackedMonsterArr(ACanTargetingActor* _target = nullptr);

	void StartHealthDecreaseCurveFTimeline();

	void StartStaminaDecreaseCurveFTimeline();

	UFUNCTION()
	void HealthDecreaseStart(float _val);

	UFUNCTION()
	void StaminaDecreaseStart(float _val);

	UFUNCTION()
	void StaminaDecreaseEnd();

	void CalStaminaRecoveryTimer();

	UFUNCTION(BlueprintCallable)
	void PlusStunCnt(bool _bReset = false);

	UFUNCTION(BlueprintCallable)
	void SetCanUseUltimate(bool _val);
};
