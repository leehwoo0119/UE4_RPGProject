// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossCombatComponent_First.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBase.h"
#include "MonsterBase_Boss.h"
#include "MonsterBase_Boss_First.h"
#include "MonsterProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "LHW/MainChar.h"
#include "MonsterWidgetComponent.h"
#include "TargetLockOnWidget.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/CharTargetLockOnComponent.h"
#include "MonsterWeaponSlot.h"
#include "MonsterTimerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/FirstBossChair.h"

UMonsterBossCombatComponent_First::UMonsterBossCombatComponent_First()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterBossCombatComponent_First::BeginPlay()
{
	Super::BeginPlay();

	monsterBase->MON_STATUS->SetMaxAttackCount(4);
	monsterBase->MON_STATUS->SetCurrentProjectileCount(0);
	monsterBase->MON_STATUS->SetMaxProjectileCount(4);
	monsterBase->MON_STATUS->SetMaxWhirlwindCnt(5);


}

void UMonsterBossCombatComponent_First::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::PhaseLoading ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::Teleport) return;

	ExecuteTakeDamageHealth(_damage);

	if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseRecorvery)
	{
		if (monsterBase->MON_STATUS->CheckOffenserseRecovery())
		{
			monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::AttackBlock);
			return;
		}
	}

	AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
	if (checkMainChar)
	{
		ExecuteTakeDamageOffenserse(_damage);
	}
	
	monsterBase->MON_STATUS->PlusCurrentGroggyCount(15);
	UE_LOG(LogTemp, Warning, TEXT("GroggyCnt: %d / %d"), monsterBase->MON_STATUS->GetCurrentGroggyCount(), monsterBase->MON_STATUS->GetMaxGroggyCount());
}


void UMonsterBossCombatComponent_First::ExecuteTakeDamageHealth(float _damage)
{
	Super::ExecuteTakeDamageHealth(_damage);


	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() <= 0)
	{
		if (monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::FirstPhase)
		{
			BossPhaseChange(true);
		}
		else
		{
			MonsterDeathProcessing();
		}
	}
}

void UMonsterBossCombatComponent_First::Attack(EAI_MeleeAttack_State _state)
{
	Super::Attack(_state);

	monsterBase->bUseControllerRotationYaw = true;

	if (monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::SecondPhase)
	{
		monsterBase->MON_STATUS->SetEnableHealthDrain(true);
	}

	MeleeAttack();
}

void UMonsterBossCombatComponent_First::AttackBlock()
{
	Super::AttackBlock();

	ExecuteTakeDamageOffenserse(monsterBase->GetMonsterStatus()->GetMaxMonsterOffensersePoint() * 0.1f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("AttackBlock"));
}

void UMonsterBossCombatComponent_First::SpecialSkill()
{
	Super::SpecialSkill();

	monsterBase->bUseControllerRotationYaw = true;
	monsterBase->MON_STATUS->SetCantParryingAttack(true);
	monsterBase->MON_STATUS->SetGuardBreakAttack(true);
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::TakeDownTypeAttack);
	int dashType = 0;

	switch (FMath::RandRange(0, 1))
	{
	case 0:
		dashType = FMath::RandRange(0, 1);
		if (dashType == 0)
		{
			monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.2f, FName("DashAttack_1_Left"));
		}
		else
		{
			monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.2f, FName("DashAttack_1_Right"));
		}
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("DashAttack_2"));
		break;
	}
	CheckSpawnElementalProjectile();
}

void UMonsterBossCombatComponent_First::ExecuteExecuted(EExecutionState_Type _state)
{
	Super::ExecuteExecuted(_state);
	
	GetWorld()->GetTimerManager().SetTimer(monsterBase->MON_TIMERCOMP->deathProssingTimer, FTimerDelegate::CreateLambda([&]()
		{
			MonsterDeathProcessing();
		}), 3.0f, false);
}

void UMonsterBossCombatComponent_First::Die()
{
	Super::Die();

	DestroyAllElementalProjectile();

	AMonsterBase_Boss_First* bossFirst = Cast<AMonsterBase_Boss_First>(GetOwner());
	check(bossFirst != nullptr);

	bossFirst->SetEyeParticle(false);
}

void UMonsterBossCombatComponent_First::BossPhaseChange(bool _bHealthZero)
{
	Super::BossPhaseChange(_bHealthZero);

	monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Idle);
	monsterBase->MON_STATEMACHINE->SetBossPhaseState(EAI_BossPhase_State::PhaseLoading);
	monsterBase->bUseControllerRotationYaw = false;

	monsterBase->CustomTimeDilation = 1.0f;
	if (bloodEffect)bloodEffect->CustomTimeDilation = 1.0f;

	//체력 오펜서스 게이지 0으로
	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(-monsterBase->MON_STATUS->GetMaxMonsterHealthPoint());
	monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(-monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint());

	monsterBase->MON_ANIM->Montage_Stop(1.0f);
	if (_bHealthZero)
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 0.7f, FName("HealthZeroPhaseChange"));
	}
	else
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 0.7f, FName("PhaseChange"));
	}

	GetWorld()->GetTimerManager().SetTimer(monsterBase->MON_TIMERCOMP->phaseChangeTimer_1, FTimerDelegate::CreateLambda([&]()
		{
			//UE_LOG(LogTemp, Warning, TEXT("Set Boss SecondPhase"));
			monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Battle);
			monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Wait);
			monsterBase->MON_STATEMACHINE->SetBossPhaseState(EAI_BossPhase_State::SecondPhase);
			monsterBase->MON_STATEMACHINE->SetBossHealthState(EAI_BossHealth_State::State_1);

			monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(monsterBase->MON_STATUS->GetMaxMonsterHealthPoint());
			monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint());
			monsterBase->MON_STATUS->SetMaxAttackCount(4);
			monsterBase->MON_STATUS->SetCurrentProjectileCount(0);
			monsterBase->MON_STATUS->SetMaxProjectileCount(3);
			monsterBase->MON_STATUS->SetCurrentGroggyCount(0);
			monsterBase->MON_STATUS->SetCheckOffenserseRecorvery(true);
		}), 5.0f, false);

	DestroyAllElementalProjectile();
	GetWorld()->GetTimerManager().SetTimer(monsterBase->MON_TIMERCOMP->phaseChangeTimer_2, FTimerDelegate::CreateLambda([&]()
	{
		monsterBase->MON_STATUS->SetMaxWhirlwindCnt(10);
	}), 5.0f, false);
}

void UMonsterBossCombatComponent_First::GroggyExecuted()
{
	Super::GroggyExecuted();

	float executeDamage = monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() * 0.15f;

	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(-executeDamage);
	monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetOffenserseZero(false);

	//인살시 체력 0이되는 상황
	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() <= 0)
	{
		if (monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::FirstPhase)
		{
			BossPhaseChange(false);
			return;
		}
		MonsterDeathProcessing();
	}
	else
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 1.0f, FName("GroggyExecuted"));
	}
}

void UMonsterBossCombatComponent_First::Groggy()
{
	DestroyAllElementalProjectile();
	monsterBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	monsterBase->MON_STATUS->SetCurrentGroggyCount(0);
	monsterBase->MON_ANIM->Montage_Stop(1.0f);
	monsterBase->MON_WEAPONSLOT->SetMonsterFirstWeaponSocket("WeaponSocket_r");
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 0.7f, FName("Groggy"));

	monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetOffenserseZero(true);
	mainChar->GetCharTargetLockOnComponent()->CheckTargetLockOn(monsterBase);
	mainChar->GetCharStatusComponent()->SetExecutionTarget(monsterBase);
}

void UMonsterBossCombatComponent_First::OffenserseRecorvery()
{
	monsterBase->bUseControllerRotationYaw = true;
	monsterBase->MON_STATUS->SetCurrentGroggyCount(0);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->offenserseRecorveryMontage, 1.0f, FName("OffenserseRecorvery"));
	monsterBase->MON_STATUS->SetCheckOffenserseRecorvery(false);
}

void UMonsterBossCombatComponent_First::OffenserseRecoverySuccess()
{
	monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(monsterBase->MON_STATUS->GetMaxMonsterOffensersePoint() / 2);
}

void UMonsterBossCombatComponent_First::GroggyEnd()
{
	if (mainChar->GetCharStatusComponent()->GetKatanaState() != EKatanaState_Type::Execution)
	{
		monsterBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	if (mainChar->GetCharStatusComponent()->GetExecutionTarget() == monsterBase)
	{
		mainChar->GetCharStatusComponent()->SetExecutionTarget(nullptr);
	}

	monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetOffenserseZero(false);
	//monsterBase->MON_WEAPONSLOT->SetMonsterFirstWeaponSocket("WeaponSocket");
}

void UMonsterBossCombatComponent_First::SpecialAttack()
{
	Super::SpecialAttack();

	monsterBase->bUseControllerRotationYaw = true;
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::SpecialTypeAttack);

	//if (!monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->executedMontage) &&
	//	!monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->hitMontage))
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.3f, FName("SpecialSlashAttack"));
	monsterBase->MON_STATEMACHINE->SetBossSpecialAttackState(EAI_BossSpecialAttack_State::AttackWait);
	CheckSpawnElementalProjectile();
}

void UMonsterBossCombatComponent_First::MeleeAttack()
{
	if (monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->executedMontage) ||
		monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->hitMontage))
		return;

	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);

	int attackType = FMath::RandRange(0, monsterBase->MON_STATUS->GetMaxAttackCount());

	switch (attackType)
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_1"));
		break;
	case 1:
		monsterBase->MON_STATUS->SetCantParryingAttack(true);
		monsterBase->MON_STATUS->SetGuardBreakAttack(true);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_2"));
		break;
	case 2:
		monsterBase->MON_STATUS->SetCantParryingAttack(true);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_3"));
		break;
	case 3:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_4"));
		break;
	case 4:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_5"));
		break;
	default:
		break;
	}

	//휠윈드 공격은 투사체를 생성하지 않는다.
	if (attackType != 2)
	{
		CheckSpawnElementalProjectile();
	}
}

void UMonsterBossCombatComponent_First::WhirlwindCnt(AMainChar* _target)
{
	monsterBase->MON_STATUS->PlusOrResetWhirlwindCnt();
	switch (monsterBase->MON_BOSSPHASESTATE)
	{
	case EAI_BossPhase_State::FirstPhase:
		monsterBase->bUseControllerRotationYaw = false;
		MonsterMoveCompTo(200.0f, false);
		break;
	case EAI_BossPhase_State::SecondPhase:
		MonsterMoveCompTo(1.0f, false, _target->GetActorLocation());
		break;
	default:
		break;
	}		

	if (monsterBase->MON_STATUS->GetMaxWhirlwindCnt() <= monsterBase->MON_STATUS->GetWhirlwindCnt())
	{
		switch (monsterBase->MON_BOSSPHASESTATE)
		{
		case EAI_BossPhase_State::FirstPhase:
			monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_3_End"));
			break;
		case EAI_BossPhase_State::SecondPhase:
			monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_3_PowerEnd"));
			break;
		default:
			break;
		}
		monsterBase->MON_STATUS->PlusOrResetWhirlwindCnt(true);
	}
}
