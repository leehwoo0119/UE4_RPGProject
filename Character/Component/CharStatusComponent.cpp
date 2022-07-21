// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStatusComponent.h"
#include "CharWidgetComponent.h"
#include "MainChar.h"

// Sets default values for this component's properties
UCharStatusComponent::UCharStatusComponent():
stealthTarget(nullptr),
bTargetLockOn(false),
bCanStealth(false),
bDefense(false),
bMoveStop(false),
bStaminaPointRecovery(true),
bInvincibility(false),
bCanUseUltimate(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	katanaState = EKatanaState_Type::Normal;
	prevKatanaState = EKatanaState_Type::Normal;
	bowState = EBowState_Type::Normal;
	prevBowState = EBowState_Type::Normal;
	dashState = EDashState_Type::BackDash;
	stealthState = EStealthState_Type::Default;
	executionState = EExecutionState_Type::Default;
	weaponTypeState = EWeaponState_Type::Katana;
	prevWeaponTypeState = EWeaponState_Type::Katana;
	bloodTypeDirectionState = EBloodDirectionState_Type::Default;
	attackAvoidState = EAttackAvoidState_Type::Default;
	targetSwitchState = ETargetSwitchState_Type::NotSwitch;

	meleeAttackCnt = 0;
	behaviorStaminaCost = 20.0f;

	maxCharHealthPoint = 100.0f;
	maxCharStaminaPoint = 100.0f;
	maxCharRecoveryPoint = 100.0f;
	maxCharUltimatePoint = 100.0f;
	curCharHealthPoint = maxCharHealthPoint;
	calCharHealthPoint = maxCharHealthPoint;
	curCharStaminaPoint = maxCharStaminaPoint;
	calCharStaminaPoint = maxCharStaminaPoint;
	curCharRecoveryPoint = 0.0f;
	curCharUltimatePoint = 0.0f;

	recoveryStaminaValue = 1.5f;

	healthDecreaseCurveLength = 2.5f;
	staminaDecreaseCurveLength = 5.0f;
}

void UCharStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
	mainChar = Cast<AMainChar>(GetOwner());
	check(mainChar != nullptr);

	check(healthDecreaseCurveFloat != nullptr);
	FOnTimelineFloat healthDecreaseCallback;
	healthDecreaseCallback.BindUFunction(this, FName("HealthDecreaseStart"));
	healthDecreaseCurveFTimeline.AddInterpFloat(healthDecreaseCurveFloat, healthDecreaseCallback);
	healthDecreaseCurveFTimeline.SetTimelineLength(healthDecreaseCurveLength);

	check(staminaDecreaseCurveFloat != nullptr);
	FOnTimelineFloat staminaDecreaseCallback;
	FOnTimelineEventStatic staminaDecreaseFinishedCallback;
	staminaDecreaseCallback.BindUFunction(this, FName("StaminaDecreaseStart"));
	staminaDecreaseFinishedCallback.BindUFunction(this, FName("StaminaDecreaseEnd"));
	staminaDecreaseCurveFTimeline.AddInterpFloat(staminaDecreaseCurveFloat, staminaDecreaseCallback);
	staminaDecreaseCurveFTimeline.SetTimelineFinishedFunc(staminaDecreaseFinishedCallback);
	staminaDecreaseCurveFTimeline.SetTimelineLength(staminaDecreaseCurveLength);
}

void UCharStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	healthDecreaseCurveFTimeline.TickTimeline(DeltaTime);
	staminaDecreaseCurveFTimeline.TickTimeline(DeltaTime);
}

void UCharStatusComponent::SetCalCharHealthPoint(float _val)
{
	calCharHealthPoint = _val;
}

void UCharStatusComponent::SetKatanaState(EKatanaState_Type _state)
{
	prevKatanaState = katanaState;
	katanaState = _state;
}

void UCharStatusComponent::SetBowState(EBowState_Type _state)
{
	if (bowState != EBowState_Type::TakeDamage)
	{
		prevBowState = bowState;
	}
	bowState = _state;
}

void UCharStatusComponent::SetDashState(EDashState_Type _state)
{
	dashState = _state;
}

void UCharStatusComponent::SetStealthState(EStealthState_Type _state)
{
	stealthState = _state;
}

void UCharStatusComponent::SetExecutionState(EExecutionState_Type _state)
{
	executionState = _state;
}

void UCharStatusComponent::SetBloodTypeDirectionState(EBloodDirectionState_Type _state)
{
	bloodTypeDirectionState = _state;
}

void UCharStatusComponent::SetDamageDirection(float _val)
{
	damageDirection = _val;
}

void UCharStatusComponent::SetDamage(float _val)
{
	damage = _val;
}

bool UCharStatusComponent::PlusOrSetCurCharHealthPoint(float _val, bool _bSetVal)
{
	if (!_bSetVal)
	{
		curCharHealthPoint += _val;
		StartHealthDecreaseCurveFTimeline();
		
		if (curCharHealthPoint <= 0)
		{
			curCharHealthPoint = 0;
			return true;
		}
		if (curCharHealthPoint >= 100)
		{
			curCharHealthPoint = 100;
		}
		return false;
	}
	else
	{
		curCharHealthPoint = _val;
		return true;
	}
}

void UCharStatusComponent::PlusOrSetCurCharStaminaPoint(float _val, bool _bSetVal)
{
	if (!_bSetVal)
	{
		if (_val < 0)
		{
			CalStaminaRecoveryTimer();
			staminaDecreaseCurveFTimeline.Stop();
		}
		curCharStaminaPoint += _val;

		if (curCharStaminaPoint <= 0.0f)
		{
			curCharStaminaPoint = 0.0f;
		}
		if (curCharStaminaPoint >= 100.0f)
		{
			bStaminaPointRecovery = false;
			curCharStaminaPoint = 100.0f;
		}
	}
	else
	{
		curCharStaminaPoint = _val;
	}
}

void UCharStatusComponent::PlusOrSetCurCharRecoveryPoint(float _val, bool _bSetVal)
{
	if (_bSetVal)
	{
		curCharRecoveryPoint = _val;
		return;
	}
	curCharRecoveryPoint += _val;
	if (curCharRecoveryPoint <= 0)curCharRecoveryPoint = 0;
	if (curCharRecoveryPoint >= 100)curCharRecoveryPoint = 100;
}

void UCharStatusComponent::PlusOrSetCurCharUltimatePoint(float _val, bool _bSetVal)
{
	if (!bCanUseUltimate)return;

	if (_bSetVal)
	{
		curCharUltimatePoint = _val;
		return;
	}
	curCharUltimatePoint += _val;
	if (curCharUltimatePoint <= 0)curCharUltimatePoint = 0;
	if (curCharUltimatePoint >= 100)curCharUltimatePoint = 100;
}

void UCharStatusComponent::SetWeaponState(EWeaponState_Type _state)
{
	if (weaponTypeState != EWeaponState_Type::Unarmed)
	{
		prevWeaponTypeState = weaponTypeState;
	}
	weaponTypeState = _state;
}

void UCharStatusComponent::SetStealthTarget(AMonsterBase_Normal* _target)
{
	stealthTarget = _target;
}

void UCharStatusComponent::SetExecutionTarget(AMonsterBase* _target)
{
	executionTarget = _target;
}

void UCharStatusComponent::SetAttackAvoidTarget(AMonsterBase* _target)
{
	attackAvoidTarget = _target;
}

void UCharStatusComponent::SetTargetLockOn(bool _val)
{
	bTargetLockOn = _val;
}

void UCharStatusComponent::SetCanStealth(bool _val)
{
	bCanStealth = _val;
}

void UCharStatusComponent::SetDefense(bool _val)
{
	bDefense = _val;
}

void UCharStatusComponent::SetMoveStop(bool _val)
{
	bMoveStop = _val;
}

void UCharStatusComponent::SetStaminaPointRecovery(bool _val)
{
	bStaminaPointRecovery = _val;
}

void UCharStatusComponent::SetInvincibility(bool _val)
{
	bInvincibility = _val;
}

void UCharStatusComponent::SetAttackAvoid(EAttackAvoidState_Type _val)
{
	attackAvoidState = _val;
}

void UCharStatusComponent::SetTargetSwitchState(ETargetSwitchState_Type _val)
{
	targetSwitchState = _val;
}

void UCharStatusComponent::PlusMeleeAttackCnt()
{
	meleeAttackCnt += 1;
	meleeAttackCnt %= 4;
}

void UCharStatusComponent::ResetMeleeAttack()
{
	meleeAttackCnt = 0;
}

bool UCharStatusComponent::CheckCanUseStaminaBehavior()
{
	if (curCharStaminaPoint <= behaviorStaminaCost)
	{
		return false;
	}

	return true;
}

bool UCharStatusComponent::CheckCanUseRecoveryBehavior()
{
	if (curCharRecoveryPoint >= maxCharRecoveryPoint)
	{
		return true;
	}

	return false;
}

void UCharStatusComponent::AddAttackedMonsterArr(ACanTargetingActor* _target)
{
	attackedMonsterArr.AddUnique(_target);
}

void UCharStatusComponent::DeleteAttackedMonsterArr(ACanTargetingActor* _target)
{
	if (!_target)
	{
		attackedMonsterArr.Empty();
	}
	else
	{
		attackedMonsterArr.Remove(_target);
	}
}

void UCharStatusComponent::StartHealthDecreaseCurveFTimeline()
{
	healthDecreaseCurveFTimeline.PlayFromStart();
}

void UCharStatusComponent::StartStaminaDecreaseCurveFTimeline()
{	
	staminaDecreaseCurveFTimeline.PlayFromStart();		
}

void UCharStatusComponent::HealthDecreaseStart(float _val)
{
	calCharHealthPoint = FMath::Lerp(calCharHealthPoint, curCharHealthPoint, _val);
}

void UCharStatusComponent::StaminaDecreaseStart(float _val)
{
	calCharStaminaPoint = FMath::Lerp(calCharStaminaPoint, curCharStaminaPoint, _val);
}

void UCharStatusComponent::StaminaDecreaseEnd()
{
	calCharStaminaPoint = maxCharStaminaPoint;
}

void UCharStatusComponent::CalStaminaRecoveryTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(calStaminaPointRecoveryTimer);

	GetWorld()->GetTimerManager().SetTimer(calStaminaPointRecoveryTimer, FTimerDelegate::CreateLambda([&]()
		{
			StartStaminaDecreaseCurveFTimeline();
		}), 1.0f, false);
}

void UCharStatusComponent::PlusStunCnt(bool _bReset)
{
	if (_bReset)stunCnt = 0;
	else stunCnt++;
}

void UCharStatusComponent::SetCanUseUltimate(bool _val)
{
	bCanUseUltimate = _val;
}

