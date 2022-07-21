// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormal_GS_CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterStateMachine.h"
#include "MonsterBase.h"
#include "MonsterNormalStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBase_Normal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "MonsterTimerComponent.h"
#include "LHW/MainChar.h"

UMonsterNormal_GS_CombatComponent::UMonsterNormal_GS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterNormal_GS_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMonsterNormal_GS_CombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero) return;

	AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
	if (checkMainChar)
	{
		ExecuteTakeDamageOffenserse(-_damage);
	}

	if ((monsterBase->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::DefenseSuccess) &&
		CheckTargetFront(_damageCauser->GetActorLocation(),
			monsterBase->GetActorLocation(),
			monsterBase->GetActorForwardVector()))
	{
		monsterBase->MON_STATUS->SetDefenseCnt(monsterBase->MON_STATUS->GetDefenseCnt() - 1);
		//UE_LOG(LogTemp, Log, TEXT("%d"), monsterBase->GetMonsterStatus()->GetDefenseCnt());
		if (monsterBase->MON_STATUS->GetDefenseCnt() == 0)
		{
			monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::DefenseEnd);
			DefenseEnd();
		}
		else
		{
			monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::DefenseSuccess);
			DefenseSuccess();
		}

		monsterBase->MON_WEAPONSLOT->GetMonsterFirstWeapon()->MonsterWeaponDefenseParticles();
		return;
	}

	FRotator findLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), _damageCauser->GetActorLocation());
	float hitDirection =
		UKismetMathLibrary::NormalizedDeltaRotator(
			GetOwner()->GetActorRotation(), findLookAtRot).Yaw;

	ExecuteTakeDamageHealth(_damage);
	TakeDamageCheckDir(hitDirection);	
}

bool UMonsterNormal_GS_CombatComponent::CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec)
{
	_vecA = FVector(_vecA.X, _vecA.Y, 0.0f);
	_vecB = FVector(_vecB.X, _vecB.Y, 0.0f);

	float dot = FVector::DotProduct((_vecA - _vecB).GetSafeNormal(), _forwardVec);
	//UE_LOG(LogTemp, Log, TEXT("%f"), dot);
	if (dot >= 0.0f)
		return true;
	else
		return false;
}

void UMonsterNormal_GS_CombatComponent::Attack(EAI_MeleeAttack_State _state)
{
	Super::Attack(_state);
	UE_LOG(LogTemp, Log, TEXT("Attack"));

	monsterBase->bUseControllerRotationYaw = true;
	switch (_state)
	{
	case EAI_MeleeAttack_State::AttackGroup_1:
		MeleeAttackGroup_1();
		break;
	case EAI_MeleeAttack_State::AttackGroup_2:
		MeleeAttackGroup_2();
		break;
	default:
		break;
	}
}

void UMonsterNormal_GS_CombatComponent::MeleeAttackGroup_1()
{
	Super::MeleeAttackGroup_1();

	monsterBase->MON_STATEMACHINE->SetMeleeAttackState(EAI_MeleeAttack_State::AttackGroup_2);
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);

	switch (FMath::RandRange(0, maxMeleeAttackValue))
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_1"));
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_2"));
		break;
	case 2:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_3"));
		break;
	case 3:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_4"));
		break;
	default:
		break;
	}
}

void UMonsterNormal_GS_CombatComponent::MeleeAttackGroup_2()
{
	Super::MeleeAttackGroup_2();

	monsterBase->MON_STATEMACHINE->SetMeleeAttackState(EAI_MeleeAttack_State::AttackGroup_1);
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);

	switch (FMath::RandRange(0, 1))
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_5"));
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_6"));
		break;
	default:
		break;
	}
}

void UMonsterNormal_GS_CombatComponent::AttackBlock()
{
	Super::AttackBlock();

	monsterBase->MON_STATUS->SetDefenseCnt(0);
	ExecuteTakeDamageOffenserse(monsterBase->GetMonsterStatus()->GetMaxMonsterOffensersePoint() * -0.1f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("AttackBlock"));
}

void UMonsterNormal_GS_CombatComponent::SpecialSkill()
{
	Super::SpecialSkill();

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::SpecialSkill);

	GetWorld()->GetTimerManager().SetTimer(
		monsterBase->MON_TIMERCOMP->defenseEndTimer, this, &UMonsterNormal_GS_CombatComponent::TimerDefenseEnd, 2.f, false);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("Defense"));
}

void UMonsterNormal_GS_CombatComponent::DefenseEnd()
{
	Super::DefenseEnd();

	monsterBase->MON_STATUS->SetDefenseCnt(0);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 2.0f, FName("DefenseEnd"));
}

void UMonsterNormal_GS_CombatComponent::TimerDefenseEnd()
{
	Super::TimerDefenseEnd();

	//UE_LOG(LogTemp, Log, TEXT("DefenseEndCall"));
	if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::DefenseSuccess)
	{
		//UE_LOG(LogTemp, Log, TEXT("DefenseEnd"));
		monsterBase->MON_STATUS->SetDefenseCnt(0);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("TimerDefenseEnd"));
	}
}

void UMonsterNormal_GS_CombatComponent::DefenseSuccess()
{
	Super::DefenseSuccess();

	//animInstance->Montage_Stop(1.0f, NULL);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialSkillMontage, 1.0f, FName("DefenseSuccess"));
}

void UMonsterNormal_GS_CombatComponent::LookAround(bool _stop)
{
	if (_stop)
		monsterBase->StopAnimMontage(monsterBase->MON_ANIM->lookAroundMontage);
	else
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->lookAroundMontage, 1.0f, FName("LookAround"));
}
