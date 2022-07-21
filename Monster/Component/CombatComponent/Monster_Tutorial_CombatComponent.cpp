// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Tutorial_CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "LHW/MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "TutorialManager.h"

UMonster_Tutorial_CombatComponent::UMonster_Tutorial_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIgnoreDamage = true;
}

void UMonster_Tutorial_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	ATutorialManager* tutorialManager = Cast<ATutorialManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATutorialManager::StaticClass()));
	tutorialManager->SetMonsterBase(monsterBase);
}

void UMonster_Tutorial_CombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	FRotator findLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), _damageCauser->GetActorLocation());
	float hitDirection =
		UKismetMathLibrary::NormalizedDeltaRotator(
			GetOwner()->GetActorRotation(), findLookAtRot).Yaw;
	TakeDamageCheckDir(hitDirection);

	if (GetIgnoreDamage())
	{
		if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() <= 50.0f ||
			monsterBase->MON_STATUS->GetCurMonsterHealthPoint() <= 50.0f)
		{
			monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(monsterBase->MON_STATUS->GetMaxMonsterHealthPoint(), true);
			monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(0.0f, monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint());
			return;
		}
	}

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero) return;

	AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
	if (checkMainChar)
	{
		ExecuteTakeDamageOffenserse(-_damage);
	}	

	ExecuteTakeDamageHealth(_damage / 2);

	if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(recoveryHandle, this, &UMonster_Tutorial_CombatComponent::RecoveryAll, 2.1f, false);
	}
}

void UMonster_Tutorial_CombatComponent::Attack(EAI_MeleeAttack_State _state)
{
	Super::Attack(_state);

	monsterBase->bUseControllerRotationYaw = true;
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_2"));
}

void UMonster_Tutorial_CombatComponent::AttackBlock()
{
	Super::AttackBlock();

	ExecuteTakeDamageOffenserse(-20.0f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("AttackBlock"));
}

void UMonster_Tutorial_CombatComponent::SetIgnoreDamage(bool _value)
{
	bIgnoreDamage = _value;
}

void UMonster_Tutorial_CombatComponent::RecoveryAll()
{
	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(monsterBase->MON_STATUS->GetMaxMonsterHealthPoint(), true);
	monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(0.0f, monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint());
	GetWorld()->GetTimerManager().ClearTimer(recoveryHandle);
}
