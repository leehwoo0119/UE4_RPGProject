// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormal_CM_CombatComponent.h"
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
#include "Kismet/KismetSystemLibrary.h"
#include "LHW/MainChar.h"
#include "MonsterProjectile.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "MonsterWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "LHW/MainChar.h"

UMonsterNormal_CM_CombatComponent::UMonsterNormal_CM_CombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    teleportEffectTime = 1.0f;
}

void UMonsterNormal_CM_CombatComponent::BeginPlay()
{
    Super::BeginPlay();

    check(teleportEffectCurveFloat != nullptr);

    FOnTimelineFloat DrawTeleportCallback;

    DrawTeleportCallback.BindUFunction(this, FName("TeleportEffectOpacity"));

    teleportEffectCurveFTimeline.AddInterpFloat(teleportEffectCurveFloat, DrawTeleportCallback);
    teleportEffectCurveFTimeline.SetTimelineLength(teleportEffectTime);
}

void UMonsterNormal_CM_CombatComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
    Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

    teleportEffectCurveFTimeline.TickTimeline(_deltaTime);
}

void UMonsterNormal_CM_CombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
    Super::TakeDamage(_damagedActor, _damage, _damageType, _instigatedBy, _damageCauser);

    if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
        monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero) return;

    AMainChar* checkMainChar = Cast<AMainChar>(_damageCauser);
    if (checkMainChar)
    {
        ExecuteTakeDamageOffenserse(-_damage);
    }  

    if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill)return;

    FRotator findLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), _damageCauser->GetActorLocation());
    float hitDirection =
        UKismetMathLibrary::NormalizedDeltaRotator(
            GetOwner()->GetActorRotation(), findLookAtRot).Yaw;

    ExecuteTakeDamageHealth(_damage);

    if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::SpecialSkill)return;
    TakeDamageCheckDir(hitDirection);
}

void UMonsterNormal_CM_CombatComponent::Attack(EAI_MeleeAttack_State _state)
{
    Super::Attack(_state);

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

void UMonsterNormal_CM_CombatComponent::MeleeAttackGroup_1()
{
    Super::MeleeAttackGroup_1();

    monsterBase->MON_STATEMACHINE->SetMeleeAttackState(EAI_MeleeAttack_State::AttackGroup_2);


    switch (FMath::RandRange(0, 1))
    {
    case 0:
        monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);
        monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_1"));
        break;
    case 1:
        monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::TakeDownTypeAttack);
        monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_2"));
        break;
    default:
        break;
    }
}

void UMonsterNormal_CM_CombatComponent::MeleeAttackGroup_2()
{
    Super::MeleeAttackGroup_2();

    monsterBase->MON_STATEMACHINE->SetMeleeAttackState(EAI_MeleeAttack_State::AttackGroup_1);
    monsterBase->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);

    switch (FMath::RandRange(0, 1))
    {
    case 0:
        monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_3"));
        break;
    case 1:
        monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 0.9f, FName("Attack_4"));
        break;
    default:
        break;
    }

}

void UMonsterNormal_CM_CombatComponent::AttackBlock()
{
    Super::AttackBlock();

    monsterBase->MON_STATUS->SetDefenseCnt(0);
    ExecuteTakeDamageOffenserse(monsterBase->GetMonsterStatus()->GetMaxMonsterOffensersePoint() * -0.1f);
    monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->attackMontage, 1.0f, FName("AttackBlock"));
}

void UMonsterNormal_CM_CombatComponent::SpecialSkill()
{
    Super::SpecialSkill();

    if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::Teleport) return;

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

void UMonsterNormal_CM_CombatComponent::LookAround(bool _stop)
{
    if (_stop)
        monsterBase->StopAnimMontage(monsterBase->MON_ANIM->lookAroundMontage);
    else
        monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->lookAroundMontage, 1.0f, FName("LookAround"));
}

void UMonsterNormal_CM_CombatComponent::SpawnSlashProjectile()
{
    Super::SpawnSlashProjectile();

    if (monsterBase->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::Die)return;

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this->GetOwner();

    FVector spawnLocation = (GetOwner()->GetActorForwardVector() * 150.0f) + GetOwner()->GetActorLocation();
    FRotator spawnRotation = GetOwner()->GetActorRotation();

    AMonsterProjectile* projectile =
        GetWorld()->SpawnActor<AMonsterProjectile>(slashProjectile, spawnLocation, spawnRotation, spawnParams);
}

void UMonsterNormal_CM_CombatComponent::TeleportEffectOpacityStart()
{
    Super::TeleportEffectOpacityStart();

    monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Teleport);
    monsterBase->GetCapsuleComponent()->SetGenerateOverlapEvents(false);
    monsterBase->GetMonsterWidgetComponent()->SetMonsterStateImageVisible(true);
    monsterBase->GetMonsterPointBarWidget()->SetVisibility(false);
    teleportEffectCurveFTimeline.PlayFromStart();
}

void UMonsterNormal_CM_CombatComponent::TeleportEffectOpacityReverse()
{
    Super::TeleportEffectOpacityReverse();

    monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Attack);
    monsterBase->GetCapsuleComponent()->SetGenerateOverlapEvents(true);
    monsterBase->GetMonsterWidgetComponent()->SetMonsterStateImageVisible(false);
    monsterBase->GetMonsterPointBarWidget()->SetVisibility(true);
    teleportEffectCurveFTimeline.Reverse();
}

void UMonsterNormal_CM_CombatComponent::TeleportEffectOpacity(float _val)
{
    Super::TeleportEffectOpacity(_val);

    for (int num = 0; num < monsterBase->GetMesh()->GetNumMaterials(); num++)
    {
        monsterBase->GetMesh()->CreateDynamicMaterialInstance(
            num,
            monsterBase->GetMesh()->GetMaterial(num))
            ->SetScalarParameterValue(
                FName("Appearance"),
                _val);
    }
    for (int num = 0; num < monsterBase->GetHeadComponent()->GetNumMaterials(); num++)
    {
        monsterBase->GetHeadComponent()->CreateDynamicMaterialInstance(
            num,
            monsterBase->GetHeadComponent()->GetMaterial(num))
            ->SetScalarParameterValue(
                FName("Appearance"),
                _val);
    }

    SetWeaponOpacity(_val);
}

void UMonsterNormal_CM_CombatComponent::SetWeaponOpacity(float _val)
{
    Super::SetWeaponOpacity(_val);

    for (int num = 0; num < monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->GetNumMaterials(); num++)
    {
        monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->CreateDynamicMaterialInstance(
            num,
            monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->GetMonsterWeaponMesh()->GetMaterial(num))
            ->SetScalarParameterValue(
                FName("Appearance"),
                _val);
    }
}