// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossCombatComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBase.h"
#include "MonsterBase_Boss.h"
#include "Components/BoxComponent.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/CharTargetLockOnComponent.h"
#include "MonsterPointBar.h"
#include "MonsterWidgetComponent.h"
#include "TargetLockOnWidget.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWeaponSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterTimerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "MonsterProjectile.h"

UMonsterBossCombatComponent::UMonsterBossCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonsterBossCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMonsterBossCombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

}

void UMonsterBossCombatComponent::ExecuteTakeDamageHealth(float _damage)
{
	Super::ExecuteTakeDamageHealth(_damage);

	PrintAttackParticle();
	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(-_damage);
	ChangeHealthState();
}

void UMonsterBossCombatComponent::ExecuteTakeDamageOffenserse(float _damage)
{
	Super::ExecuteTakeDamageOffenserse(_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() == 0)return;

	if(!CheckOffensersePointDecrease(-_damage))return;
	
	monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(-_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() <= 0)
	{
		OffenserseZero();
	}
}

void UMonsterBossCombatComponent::Die()
{
	Super::Die();

	DrawClearUI();
	monsterBase->MON_WIDGETCOMP->DrawBossPointBar(false);
}

void UMonsterBossCombatComponent::HealthDrain(float _value)
{
	//UE_LOG(LogTemp, Warning, TEXT("HealthDrain"));
	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() < monsterBase->MON_STATUS->GetMaxMonsterHealthPoint())
		monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(_value * 0.2f);
}

void UMonsterBossCombatComponent::ChangeHealthState()
{
	float healthPercent =
		monsterBase->MON_STATUS->GetCurMonsterHealthPoint() / monsterBase->MON_STATUS->GetMaxMonsterHealthPoint();
	if (UKismetMathLibrary::InRange_FloatFloat(healthPercent, 0.4f, 0.7f, true, false))
	{
		monsterBase->MON_STATEMACHINE->SetBossHealthState(EAI_BossHealth_State::State_2);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(healthPercent, 0.0f, 0.4f, true, false))
	{
		monsterBase->MON_STATEMACHINE->SetBossHealthState(EAI_BossHealth_State::State_3);
	}
}

bool UMonsterBossCombatComponent::CheckOffensersePointDecrease(float _damage)
{
	switch (monsterBase->MON_STATEMACHINE->GetBossHealthState())
	{
	case EAI_BossHealth_State::State_1:
		if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() + _damage <= 
			monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint() * 0.7f)
		{
			monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(
				0.0f,
				monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint() * 0.7f);
			return false;
		}
		break;
	case EAI_BossHealth_State::State_2:
		if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() + _damage <= 
			monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint() * 0.4f)
		{
			monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(
				0.0f,
				monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint() * 0.4f);
			return false;
		}
		break;
	}
	return true;
}

void UMonsterBossCombatComponent::CheckSpawnElementalProjectile()
{
	monsterBase->MON_STATUS->PlusCurrentProjectileCount();
	if (monsterBase->MON_STATUS->GetMaxProjectileCount() <= monsterBase->MON_STATUS->GetCurrentProjectileCount())
	{
		SpawnElementalProjectile();
		monsterBase->MON_STATUS->SetCurrentProjectileCount(0);
	}
}

void UMonsterBossCombatComponent::SpawnSlashProjectile()
{
	if (monsterBase->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::Die)return;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this->GetOwner();

	FVector spawnLocation = (GetOwner()->GetActorForwardVector() * 50.0f) + GetOwner()->GetActorLocation();
	FRotator spawnRotation = GetOwner()->GetActorRotation();

	AMonsterProjectile* projectile =
		GetWorld()->SpawnActor<AMonsterProjectile>(slashProjectile, spawnLocation, spawnRotation, spawnParams);
}


