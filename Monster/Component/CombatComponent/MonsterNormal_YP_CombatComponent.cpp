// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormal_YP_CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBase_Normal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterElementalProjectile.h"
#include "MonsterElementalOrb.h"
#include "MonsterElementalPillar.h"
#include "LHW/MainChar.h"

UMonsterNormal_YP_CombatComponent::UMonsterNormal_YP_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterNormal_YP_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	check(elementalProjectile_Flame != nullptr);
	check(elementalOrb_Flame != nullptr);
	check(elementalPillar_Flame != nullptr);
}

void UMonsterNormal_YP_CombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero) return;

	AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
	if (checkMainChar)
	{
		ExecuteTakeDamageOffenserse(-_damage);
	}

	FRotator findLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), _damageCauser->GetActorLocation());
	float hitDirection =
		UKismetMathLibrary::NormalizedDeltaRotator(
			GetOwner()->GetActorRotation(), findLookAtRot).Yaw;

	ExecuteTakeDamageHealth(_damage);
	TakeDamageCheckDir(hitDirection);
}

void UMonsterNormal_YP_CombatComponent::Attack(EAI_MeleeAttack_State _state)
{
	Super::Attack(_state);

	monsterBase->bUseControllerRotationYaw = true;
	switch (_state)
	{
	case EAI_MeleeAttack_State::AttackGroup_1:
		MeleeAttackGroup_1();
		break;
	default:
		break;
	}
}

void UMonsterNormal_YP_CombatComponent::MeleeAttackGroup_1()
{
	Super::MeleeAttackGroup_1();

	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::DotDamageTypeAttack);
	monsterBase->MON_STATUS->SetElementalType(EAI_ElementalType_State::Flame);

	switch (FMath::RandRange(0, 2))
	{
	case 0:
		monsterBase->MON_STATEMACHINE->SetMagicSpawnTypeState(EAI_MagicSpawnType_State::Projectile);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_1"));
		break;
	case 1:
		if (GetCurElementalOrbNum() < GetMaxElementalOrbNum())
		{
			monsterBase->MON_STATEMACHINE->SetMagicSpawnTypeState(EAI_MagicSpawnType_State::Orb);
			monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_2"));
			PlusCurElementalOrbNum(1);
		}
		else
		{
			MeleeAttackGroup_1();
		}
		break;
	case 2:
		monsterBase->MON_STATEMACHINE->SetMagicSpawnTypeState(EAI_MagicSpawnType_State::Pillar);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_3"));
		break;
	default:
		break;
	}
}

void UMonsterNormal_YP_CombatComponent::SpecialSkill()
{
	Super::SpecialSkill();

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::SpecialSkill);

	switch (FMath::RandRange(0, 3))
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("Dash_1"));
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("Dash_2"));
		break;
	case 2:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("Dash_3"));
		break;
	case 3:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("Dash_4"));
		break;
	}
}

void UMonsterNormal_YP_CombatComponent::LookAround(bool _stop)
{
	if (_stop)
		monsterBase->StopAnimMontage(monsterBase->MON_ANIM->lookAroundMontage);
	else
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->lookAroundMontage, 1.0f, FName("LookAround"));
}

void UMonsterNormal_YP_CombatComponent::SpawnElemental(FVector _fireLocation, FRotator _fireRotator, float _scale)
{
	Super::SpawnElemental(_fireLocation, _fireRotator, _scale);

	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::DotDamageTypeAttack);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this->GetOwner();

	switch (monsterBase->MON_STATEMACHINE->GetMagicSpawnTypeState())
	{
	case EAI_MagicSpawnType_State::Projectile:
	{
		AMonsterElementalProjectile* spawnProjectile
			= GetWorld()->SpawnActor<AMonsterElementalProjectile>(elementalProjectile_Flame, _fireLocation, _fireRotator, spawnParams);
		if (spawnProjectile)
			spawnProjectile->SetActorScale3D(FVector(_scale));
		break;
	}
	case EAI_MagicSpawnType_State::Orb:
	{
		AMonsterElementalOrb* spawnOrb
			= GetWorld()->SpawnActor<AMonsterElementalOrb>(elementalOrb_Flame, _fireLocation, _fireRotator, spawnParams);
		if (spawnOrb)
			spawnOrb->SetActorScale3D(FVector(_scale));
		break;
	}
	case EAI_MagicSpawnType_State::Pillar:
	{
		AMonsterElementalPillar* spawnPillar
			= GetWorld()->SpawnActor<AMonsterElementalPillar>(elementalPillar_Flame, _fireLocation, _fireRotator, spawnParams);
		break;
	}
	default:
		break;
	}
}
