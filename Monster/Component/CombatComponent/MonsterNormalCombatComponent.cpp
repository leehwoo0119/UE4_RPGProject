// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalCombatComponent.h"
#include "MonsterStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterBase_Normal.h"
#include "MonsterBaseStatus.h"
#include "MonsterController_Normal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseAnimInstance.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/CharTargetLockOnComponent.h"
#include "Components/BoxComponent.h"
#include "MonsterWidgetComponent.h"
#include "TargetLockOnWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UMonsterNormalCombatComponent::UMonsterNormalCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	curElementalOrbNum = 0;
	maxElementalOrbNum = 2;
}

void UMonsterNormalCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	switch (monsterType)
	{
	case EAI_MonsterType::Warrior:
		maxMeleeAttackValue = 3;
		break;
	case EAI_MonsterType::Guard:
		maxMeleeAttackValue = 2;
		break;
	case EAI_MonsterType::Sherden:
		maxMeleeAttackValue = 1;
		break;
	}
	monsterBase->GetStealthArea()->OnComponentBeginOverlap.AddDynamic(this, &UMonsterNormalCombatComponent::StealthAreaOnOverlapBegin);
	monsterBase->GetStealthArea()->OnComponentEndOverlap.AddDynamic(this, &UMonsterNormalCombatComponent::StealthAreaOnOverlapEnd);
}

void UMonsterNormalCombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)return;

	if (monsterBase->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::WeaponSwap)
		monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Battle);

	if (monsterBase->MON_STATEMACHINE->GetBehaviorState() != EAI_Behavior_State::Battle)
	{
		if (_damageCauser == mainChar)
		{
			monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsVector("targetLocationKey", mainChar->GetActorLocation());
			monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsObject("focusActorKey", mainChar);
			WeaponEquip();
			monsterBase->MON_CTRL->SetFocus(mainChar);
		}
		else
		{
			monsterBase->MON_CTRL->GetBlackboardComponent()->SetValueAsVector("targetLocationKey", GetInvestigatePosition());
			AMonsterController_Normal* normalController = Cast<AMonsterController_Normal>(monsterBase->GetController());
			normalController->HearingSensePause(false);
			monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Investigate);
		}
	}
}

void UMonsterNormalCombatComponent::SetInvestigatePosition(FVector _value)
{
	investigatePosition = _value;
}

void UMonsterNormalCombatComponent::ExecuteTakeDamageHealth(float _damage)
{
	Super::ExecuteTakeDamageHealth(_damage);

	PrintAttackParticle();
	monsterBase->MON_STATUS->PlusCurMonsterHealthPoint(-_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterHealthPoint() <= 0)
	{
		MonsterDeathProcessing();
	}
}

void UMonsterNormalCombatComponent::ExecuteTakeDamageOffenserse(float _damage)
{
	Super::ExecuteTakeDamageOffenserse(_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() == 0)return;

	monsterBase->MON_STATUS->PlusCurMonsterOffensersePoint(_damage);

	if (monsterBase->MON_STATUS->GetCurMonsterOffensersePoint() <= 0)
	{
		OffenserseZero();
	}
}

void UMonsterNormalCombatComponent::Die()
{
	Super::Die();

	monsterBase->GetMonsterPointBarWidget()->SetVisibility(false);
}

void UMonsterNormalCombatComponent::ExecuteStealth(EStealthState_Type _state)
{
	Super::ExecuteStealth(_state);

	monsterBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	monsterBase->GetCharacterMovement()->DisableMovement();

	switch (_state)
	{
	case EStealthState_Type::Stealth_1:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->stealthedMontage, 0.8f, FName("Stealthed_1"));
		break;
	case EStealthState_Type::Stealth_2:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->stealthedMontage, 0.8f, FName("Stealthed_2"));
		break;
	case EStealthState_Type::Stealth_3:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->stealthedMontage, 0.8f, FName("Stealthed_3"));
		break;
	case EStealthState_Type::Default:
		break;
	}
}

void UMonsterNormalCombatComponent::StealthAreaOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainChar* castMainChar = Cast<AMainChar>(OtherActor);
		AMonsterBase_Normal* monsterBaseNormal = Cast<AMonsterBase_Normal>(GetOwner());
		if (castMainChar)
		{
			if (monsterBaseNormal->MON_BEHAVIORSTATE == EAI_Behavior_State::Idle)
			{
				castMainChar->GetCharTargetLockOnComponent()->CheckTargetLockOn(monsterBase);
				monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetCanStealth(true);
				mainChar->GetCharStatusComponent()->SetStealthTarget(monsterBaseNormal);
			}			
		}
	}
}

void UMonsterNormalCombatComponent::StealthAreaOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainChar* castMainChar = Cast<AMainChar>(OtherActor);

		if (castMainChar)
		{
			if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Idle)
			{
				castMainChar->GetCharTargetLockOnComponent()->ResetTarget();
			}			
			monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetCanStealth(false);
			mainChar->GetCharStatusComponent()->SetStealthTarget(nullptr);
		}
	}
}

void UMonsterNormalCombatComponent::PlusCurElementalOrbNum(int _value)
{
	curElementalOrbNum += _value;
}