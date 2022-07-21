// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseStatus.h"
#include "MonsterBase.h"
#include "MonsterPointBar.h"
#include "MonsterHealthBar.h"
#include "LSH/MonsterWidgetComponent.h"
#include "LHW/ECharState.h"
#include "MonsterOffenserseBar.h"
#include "EAIState.h"

// Sets default values for this component's properties
UMonsterBaseStatus::UMonsterBaseStatus() :bCantParryingAttack(false), bAttackEnable(true), bCantGuardAttack(false), bPlayerSee(false), bCantGuardProjectile(false), monsterID(-1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	bloodTypeDirectionState = EBloodDirectionState_Type::Default;

	maxMonsterHealthPoint = 100.0f;
	maxMonsterOffensersePoint = 100.0f;
	pointDecreaseCurveLength = 2.0f;
	chaseDis = 600.0f;
	monsterDamage = 0.0f;
}

// Called when the game starts
void UMonsterBaseStatus::BeginPlay()
{
	Super::BeginPlay();

	curMonsterHealthPoint = maxMonsterHealthPoint;
	calMonsterHealthPoint = maxMonsterHealthPoint;
	curMonsterOffensersePoint = maxMonsterOffensersePoint;
	calMonsterOffensersePoint = maxMonsterOffensersePoint;

	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);

	//ID√ ±‚»≠ æ»µ 
	check(monsterID != -1);

	check(pointDecreaseCurveFloat != nullptr);
	FOnTimelineFloat healthDecreaseCallback;
	healthDecreaseCallback.BindUFunction(this, FName("HealthDecreaseStart"));
	healthDecreaseCurveFTimeline.AddInterpFloat(pointDecreaseCurveFloat, healthDecreaseCallback);
	healthDecreaseCurveFTimeline.SetTimelineLength(pointDecreaseCurveLength);

	FOnTimelineFloat offenserseDecreaseCallback;
	offenserseDecreaseCallback.BindUFunction(this, FName("OffenserseDecreaseStart"));
	offenserseDecreaseCurveFTimeline.AddInterpFloat(pointDecreaseCurveFloat, offenserseDecreaseCallback);
	offenserseDecreaseCurveFTimeline.SetTimelineLength(pointDecreaseCurveLength);
}

// Called every frame
void UMonsterBaseStatus::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	healthDecreaseCurveFTimeline.TickTimeline(DeltaTime);
	offenserseDecreaseCurveFTimeline.TickTimeline(DeltaTime);
}

void UMonsterBaseStatus::PlusCurMonsterHealthPoint(float _val, bool _bSet)
{	
	if (_bSet)
	{
		curMonsterHealthPoint = _val;
	}
	else
	{
		curMonsterHealthPoint += _val;
		StartHealthDecreaseCurveFTimeline();
		if (curMonsterHealthPoint <= 0) curMonsterHealthPoint = 0;
		if (maxMonsterHealthPoint < curMonsterHealthPoint) curMonsterHealthPoint = maxMonsterHealthPoint;
	}	
}

void UMonsterBaseStatus::PlusCurMonsterOffensersePoint(float _val, float _setOffensersePoint)
{
	if (_setOffensersePoint == 0.0f)
	{
		curMonsterOffensersePoint += _val;
		if (curMonsterOffensersePoint <= 0.0f)curMonsterOffensersePoint = 0.0f;
		if (maxMonsterOffensersePoint < curMonsterOffensersePoint) curMonsterOffensersePoint = maxMonsterOffensersePoint;
	}
	else
	{
		curMonsterOffensersePoint = _setOffensersePoint;
	}
	StartOffenserseDecreaseCurveFTimeline();
}

void UMonsterBaseStatus::SetCantParryingAttack(bool _value)
{
	bCantParryingAttack = _value;
}

void UMonsterBaseStatus::SetGuardBreakAttack(bool _value)
{
	bGuardBreakAttack = _value;
}

void UMonsterBaseStatus::SetCantGuardAttack(bool _value)
{
	bCantGuardAttack = _value;
}

void UMonsterBaseStatus::SetAttackEnable(bool _value)
{
	bAttackEnable = _value;
}

void UMonsterBaseStatus::SetPlayerSee(bool _value)
{
	bPlayerSee = _value;
}

void UMonsterBaseStatus::SetCantGuardProjectile(bool _value)
{
	bCantGuardProjectile = _value;
}

void UMonsterBaseStatus::SetBloodTypeDirectionState(EBloodDirectionState_Type _state)
{
	bloodTypeDirectionState = _state;
}

void UMonsterBaseStatus::StartHealthDecreaseCurveFTimeline()
{
	healthDecreaseCurveFTimeline.PlayFromStart();
}

void UMonsterBaseStatus::StartOffenserseDecreaseCurveFTimeline()
{
	offenserseDecreaseCurveFTimeline.PlayFromStart();
}

void UMonsterBaseStatus::SetChaseDis(float _val)
{
	chaseDis = _val;
}

void UMonsterBaseStatus::HealthDecreaseStart(float _val)
{
	calMonsterHealthPoint = FMath::Lerp(calMonsterHealthPoint, curMonsterHealthPoint, _val);
}

void UMonsterBaseStatus::OffenserseDecreaseStart(float _val)
{
	calMonsterOffensersePoint = FMath::Lerp(calMonsterOffensersePoint, curMonsterOffensersePoint, _val);
}

