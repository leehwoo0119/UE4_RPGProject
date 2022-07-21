// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormal_SF_CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBase_Normal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterElementalProjectile.h"
#include "LHW/MainChar.h"

UMonsterNormal_SF_CombatComponent::UMonsterNormal_SF_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMonsterNormal_SF_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	check(elementalProjectile_Flame != nullptr);
	check(elementalProjectile_Poison != nullptr);
}

void UMonsterNormal_SF_CombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
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

void UMonsterNormal_SF_CombatComponent::Attack(EAI_MeleeAttack_State _state)
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

void UMonsterNormal_SF_CombatComponent::MeleeAttackGroup_1()
{
	Super::MeleeAttackGroup_1();

	//monsterBase->MON_STATEMACHINE->SetMeleeAttackState(EAI_MeleeAttack_State::AttackGroup_2);
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::DotDamageTypeAttack);

	switch (monsterBase->MON_STATUS->GetElementalType())
	{
	case EAI_ElementalType_State::Flame:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_1"));
		break;
	case EAI_ElementalType_State::Poison:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_2"));
		break;
	default:
		break;
	}
}

void UMonsterNormal_SF_CombatComponent::LookAround(bool _stop)
{
	if (_stop)
		monsterBase->StopAnimMontage(monsterBase->MON_ANIM->lookAroundMontage);
	else
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->lookAroundMontage, 1.0f, FName("LookAround"));
}

void UMonsterNormal_SF_CombatComponent::SpawnProjectile(FVector _fireLocation, FRotator _fireRotator)
{
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::DotDamageTypeAttack);

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this->GetOwner();

	switch (monsterBase->MON_STATUS->GetElementalType())
	{
	case EAI_ElementalType_State::Flame:
		GetWorld()->SpawnActor<AMonsterElementalProjectile>(elementalProjectile_Flame, _fireLocation, _fireRotator, spawnParams);
		break;
	case EAI_ElementalType_State::Poison:
		GetWorld()->SpawnActor<AMonsterElementalProjectile>(elementalProjectile_Poison, _fireLocation, _fireRotator, spawnParams);
		break;
	default:
		break;
	}

}

