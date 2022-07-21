// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossCombatComponent_Sec.h"
#include "MonsterBase.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterStateMachine.h"
#include "MonsterTimerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "LHW/SecondBossAreaPattern.h"
#include "LHW/MainChar.h"

UMonsterBossCombatComponent_Sec::UMonsterBossCombatComponent_Sec() :teleportEffectTime(1.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterBossCombatComponent_Sec::BeginPlay()
{
	Super::BeginPlay();

	check(bossAreaPattern != nullptr);
	monsterBase->MON_STATUS->SetMaxAttackCount(3);
	monsterBase->MON_STATUS->SetCurrentProjectileCount(0);
	monsterBase->MON_STATUS->SetMaxProjectileCount(4);

	check(teleportEffectCurveFloat != nullptr);

	FOnTimelineFloat DrawTeleportCallback;

	DrawTeleportCallback.BindUFunction(this, FName("TeleportEffectOpacity"));

	teleportEffectCurveFTimeline.AddInterpFloat(teleportEffectCurveFloat, DrawTeleportCallback);
	teleportEffectCurveFTimeline.SetTimelineLength(teleportEffectTime);

}

void UMonsterBossCombatComponent_Sec::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	teleportEffectCurveFTimeline.TickTimeline(DeltaTime);
}

void UMonsterBossCombatComponent_Sec::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::PhaseLoading ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::Teleport) return;

	ExecuteTakeDamageHealth(_damage );
	AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
	if (checkMainChar)
	{
		ExecuteTakeDamageOffenserse(_damage);
	}

	BossPhaseChange(false);
}

void UMonsterBossCombatComponent_Sec::ExecuteTakeDamageHealth(float _damage)
{
	Super::ExecuteTakeDamageHealth(_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() <= 0)
	{
		MonsterDeathProcessing();
	}
}

void UMonsterBossCombatComponent_Sec::Die()
{
	Super::Die();

	bossAreaPattern->DestroyPatternArea();
}

void UMonsterBossCombatComponent_Sec::Attack(EAI_MeleeAttack_State _state)
{
	Super::Attack(_state);

	monsterBase->bUseControllerRotationYaw = true;

	if (monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->executedMontage) ||
		monsterBase->MON_ANIM->Montage_IsPlaying(monsterBase->MON_ANIM->hitMontage))
		return;

	switch (monsterBase->MON_STATEMACHINE->GetBossWeaponTypeState())
	{
	case EAI_BossWeaponType_State::Katana:
		KatanaAttack();
		break;
	case EAI_BossWeaponType_State::Mage:
		MageAttack();
		break;
	}
}

void UMonsterBossCombatComponent_Sec::SpecialSkill()
{
	Super::SpecialSkill();

	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::TakeDownTypeAttack);

	switch (monsterBase->MON_STATEMACHINE->GetBossWeaponTypeState())
	{
	case EAI_BossWeaponType_State::Katana:
		monsterBase->bUseControllerRotationYaw = true;
		monsterBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_5"));
		break;
	case EAI_BossWeaponType_State::Mage:
		monsterBase->MON_STATUS->SetGuardBreakAttack(true);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("MageAttack_7"));
		break;
	}
}

void UMonsterBossCombatComponent_Sec::SpecialAttack()
{
	Super::SpecialAttack();

	TeleportEffectOpacity(0.0f);

	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() / monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() >= 0.5f)
	{
		ChangeMageState();	
	}
	else
	{
		monsterBase->MON_STATUS->SetCantParryingAttack(true);
		monsterBase->MON_STATUS->SetCantGuardAttack(true);
		monsterBase->MON_STATEMACHINE->SetBossWeaponTypeState(EAI_BossWeaponType_State::Mage);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("MageAttack_4"));
	}	
}

void UMonsterBossCombatComponent_Sec::ExecuteAttackAvoid()
{
	monsterBase->bUseControllerRotationYaw = false;
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.15f, FName("AttackFailExecute"));
}

void UMonsterBossCombatComponent_Sec::BossPhaseChange(bool _bHealthZero)
{
	Super::BossPhaseChange(_bHealthZero);

	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() / monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() <= 0.3f &&
		monsterBase->MON_STATEMACHINE->GetBossPhaseState() == EAI_BossPhase_State::FirstPhase)
	{
		monsterBase->MON_STATEMACHINE->SetBossPhaseState(EAI_BossPhase_State::SecondPhase);
	}
}

void UMonsterBossCombatComponent_Sec::KatanaAttack()
{
	int attackType = FMath::RandRange(0, monsterBase->MON_STATUS->GetMaxAttackCount());
	
	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);
	monsterBase->MON_STATUS->SetGuardBreakAttack(false);
	monsterBase->MON_STATUS->SetCantParryingAttack(false);
	monsterBase->MON_STATUS->SetCantGuardAttack(false);

	if (monsterBase->MON_BOSSPHASESTATE == EAI_BossPhase_State::SecondPhase)
	{
		monsterBase->MON_STATUS->SetEnableHealthDrain(true);
	}
	//attackType = 3;
	switch (attackType)
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_1"));
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_2"));
		break;
	case 2:
		monsterBase->MON_STATUS->SetGuardBreakAttack(true);
		monsterBase->MON_STATUS->SetCantParryingAttack(true);
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_3"));
		break;
	case 3:
		monsterBase->MON_STATUS->SetCanAvoidAttack(true);
		monsterBase->bUseControllerRotationYaw = false;
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("Attack_4"));
		break;
	default:
		break;
	}

	if (attackType != 3)
	{
		CheckSpawnElementalProjectile();
	}
}

void UMonsterBossCombatComponent_Sec::MageAttack()
{
	int attackType = FMath::RandRange(0, 2);

	monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::TakeDownTypeAttack);
	monsterBase->MON_STATUS->SetGuardBreakAttack(true);
	monsterBase->MON_STATUS->SetCantParryingAttack(true);
	monsterBase->MON_STATUS->SetCantGuardAttack(true);

	switch (attackType)
	{
	case 0:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("Dodge_B"));
		break;
	case 1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("Dodge_R"));
		break;
	case 2:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("Dodge_L"));
		break;
	default:
		break;
	}
	monsterBase->MON_STATUS->PlusMageAttackCnt();

	if (monsterBase->MON_STATUS->GetMageAttackCnt() > monsterBase->MON_STATUS->GetMaxMageAttackCnt())
	{
		CheckChangeMageState();
	}
}

void UMonsterBossCombatComponent_Sec::AttackBlock()
{
	Super::AttackBlock();

	switch (monsterBase->MON_STATEMACHINE->GetBossWeaponTypeState())
	{
	case EAI_BossWeaponType_State::Katana:
		KatanaAttackBlock();
		break;
	case EAI_BossWeaponType_State::Mage:
		MageAttackBlock();
		break;
	}

}

void UMonsterBossCombatComponent_Sec::KatanaAttackBlock()
{
	ExecuteTakeDamageOffenserse(monsterBase->GetMonsterStatus()->GetMaxMonsterOffensersePoint() * 0.1f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("AttackBlock"));
}

void UMonsterBossCombatComponent_Sec::MageAttackBlock()
{
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 1.0f, FName("AttackBlock"));
}

void UMonsterBossCombatComponent_Sec::CheckChangeMageState()
{
	int randNum = FMath::RandRange(1, 100);
	//UE_LOG(LogTemp, Log, TEXT("%d"), randNum);
	if (randNum < 40.0f)
	{
		monsterBase->MON_STATUS->PlusMageAttackCnt(true);
		ChangeKatanaState();
	}
}

void UMonsterBossCombatComponent_Sec::ChangeMageState()
{
	monsterBase->MON_STATEMACHINE->SetBossWeaponTypeState(EAI_BossWeaponType_State::Mage);
	monsterBase->MON_STATUS->SetChaseDis(800.0f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 0.6f, FName("ChangeMageState"));
	SetWeaponOpacity(1.0f);
}

void UMonsterBossCombatComponent_Sec::ChangeKatanaState()
{
	monsterBase->MON_STATEMACHINE->SetBossWeaponTypeState(EAI_BossWeaponType_State::Katana);
	monsterBase->MON_STATUS->SetChaseDis(600.0f);
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->specialAttackMontage, 0.6f, FName("ChangeKatanaState"));
	SetWeaponOpacity(0.0f);
}

void UMonsterBossCombatComponent_Sec::ExecuteInstantDeath()
{
	DestroyAllElementalProjectile();
	monsterBase->MON_STATEMACHINE->SetBossWeaponTypeState(EAI_BossWeaponType_State::Katana);
	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::InstantKill);
	SetWeaponOpacity(0.0f);
	monsterBase->GetCharacterMovement()->StopMovementImmediately();
	monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.7f, FName("ExecuteInstantDeath"));
}

void UMonsterBossCombatComponent_Sec::TeleportEffectOpacityStart()
{
	Super::TeleportEffectOpacityStart();

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Teleport);
	teleportEffectCurveFTimeline.PlayFromStart();
}

void UMonsterBossCombatComponent_Sec::TeleportEffectOpacityReverse()
{
	Super::TeleportEffectOpacityReverse();

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Attack);
	teleportEffectCurveFTimeline.Reverse();
}

void UMonsterBossCombatComponent_Sec::TeleportEffectOpacity(float _val)
{
	Super::TeleportEffectOpacity(_val);

	for (int num = 0; num < monsterBase->GetMesh()->GetNumMaterials(); num++)
	{
		monsterBase->GetMesh()->CreateDynamicMaterialInstance(
			num,
			monsterBase->GetMesh()->GetMaterial(num))
			->SetScalarParameterValue(
				FName("Appearance"),
				_val * 2);
	}
	for (int num = 0; num < monsterBase->GetHeadComponent()->GetNumMaterials(); num++)
	{
		monsterBase->GetHeadComponent()->CreateDynamicMaterialInstance(
			num,
			monsterBase->GetHeadComponent()->GetMaterial(num))
			->SetScalarParameterValue(
				FName("Appearance"),
				_val * 2);
	}

	SetWeaponOpacity(_val);
}

void UMonsterBossCombatComponent_Sec::SetWeaponOpacity(float _val)
{
	Super::SetWeaponOpacity(_val);

	if (monsterBase->MON_STATEMACHINE->GetBossWeaponTypeState() == EAI_BossWeaponType_State::Mage)
	{
		_val = 1.0f;
	}
	for (int num = 0; num < monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->GetNumMaterials(); num++)
	{
		monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->CreateDynamicMaterialInstance(
			num,
			monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->GetMaterial(num))
			->SetScalarParameterValue(
				FName("Appearance"),
				_val * 2);
	}
}

void UMonsterBossCombatComponent_Sec::ExecuteHealthDrain()
{	
	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(10.0f);

	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() / monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() > 0.5f)
	{
		monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() * 0.5f, true);
	}
}
