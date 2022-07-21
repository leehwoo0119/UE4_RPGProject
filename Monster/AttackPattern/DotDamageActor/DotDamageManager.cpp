// Fill out your copyright notice in the Description page of Project Settings.


#include "DotDamageManager.h"
#include "LHW/MainChar.h"
#include "LHW/CharCombatComponent.h"
#include "MonsterBase.h"
#include "MonsterBase_Normal.h"
#include "MonsterBaseController.h"
#include "MonsterBaseStatus.h"
#include "Kismet/GameplayStatics.h"
#include "DotDamageActor.h"
#include "DotDamageActor_Flame.h"
#include "DotDamageActor_Poison.h"
#include "DotDamageActor_Frozen.h"
#include "LHW/ECharState.h"
#include "LHW/CharStatusComponent.h"
#include "MonsterProjectile.h"
#include "MonsterElementalOrb.h"

ADotDamageManager::ADotDamageManager()
{
    PrimaryActorTick.bCanEverTick = false;

}

void ADotDamageManager::BeginPlay()
{
    Super::BeginPlay();

    mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ADotDamageManager::CreateParticleActor(AActor* _target, float _targetMaxHealth, FVector const& _location, FRotator const& _rotation, AMonsterBase* _monsterBase, EAI_ElementalType_State _elementalType, const FActorSpawnParameters& _spawnParameters)
{
    // 플레이어가 사망하면 더 이상 도트 데미지 액터를 생성하지 않음
    if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Die ||
        mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Die)
        return;

    AMainChar* hitMainChar = Cast<AMainChar>(_target);
    AMonsterBase_Normal* hitMonsterBase_Normal = Cast<AMonsterBase_Normal>(_target);


    switch (_elementalType)
    {
    case EAI_ElementalType_State::Flame:
    {
        if (hitMainChar)
        {
            TArray<AActor*> attachedActors;
            hitMainChar->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Flame")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetFlameDamageNum(0);
                    return;
                }
            }
        }

        if (hitMonsterBase_Normal)
        {
            TArray<AActor*> attachedActors;
            hitMonsterBase_Normal->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Flame")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetFlameDamageNum(0);
                    return;
                }
            }
        }

        ADotDamageActor_Flame* flameEffect =
            GetWorld()->SpawnActor<ADotDamageActor_Flame>(DotDamageActor_Flame, _location, _rotation, _spawnParameters);
        flameEffect->SetActorScale3D(FVector(0.3f));
        AddDotDamageActorArray(flameEffect);

        break;
    }
    case EAI_ElementalType_State::Poison:
    {
        if (hitMainChar)
        {
            TArray<AActor*> attachedActors;
            hitMainChar->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Poison")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetPoisonDotTimerNum(7);
                    dotDamageActor->PlusPoisonStack();
                    return;
                }
            }
        }

        if (hitMonsterBase_Normal)
        {
            TArray<AActor*> attachedActors;
            hitMonsterBase_Normal->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Poison")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetPoisonDotTimerNum(7);
                    dotDamageActor->PlusPoisonStack();
                    return;
                }
            }
        }

        ADotDamageActor_Poison* poisonEffect =
            GetWorld()->SpawnActor<ADotDamageActor_Poison>(DotDamageActor_Poison, _location, _rotation, _spawnParameters);
        poisonEffect->SetActorScale3D(FVector(0.3f));
        AddDotDamageActorArray(poisonEffect);

        break;
    }
    case EAI_ElementalType_State::Frozen:
    {
        if (hitMainChar)
        {
            TArray<AActor*> attachedActors;
            hitMainChar->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Frozen")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetFrozenCCTimerNum(7);
                    dotDamageActor->PlusFrozenStack();
                    return;
                }
            }
        }

        if (hitMonsterBase_Normal)
        {
            TArray<AActor*> attachedActors;
            hitMonsterBase_Normal->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Frozen")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetFrozenCCTimerNum(7);
                    dotDamageActor->PlusFrozenStack();
                    return;
                }
            }
        }

        ADotDamageActor_Frozen* frozenEffect =
            GetWorld()->SpawnActor<ADotDamageActor_Frozen>(DotDamageActor_Frozen, _location, _rotation, _spawnParameters);
        frozenEffect->SetActorScale3D(FVector(0.3f));
        AddDotDamageActorArray(frozenEffect);

        break;
    }
    default:
    {
        // default는 보스가 생성하는 화염구 데미지 처리를 위해 존재함
        if (hitMainChar)
        {
            TArray<AActor*> attachedActors;
            hitMainChar->GetAttachedActors(attachedActors);
            for (auto* attached : attachedActors)
            {
                if (attached->ActorHasTag(FName("DotDamage_Flame")))
                {
                    ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(attached);
                    dotDamageActor->SetFlameDamageNum(0);
                    return;
                }
            }
        }
        ADotDamageActor_Flame* flameEffect =
            GetWorld()->SpawnActor<ADotDamageActor_Flame>(DotDamageActor_Flame, _location, _rotation, _spawnParameters);
        flameEffect->SetActorScale3D(FVector(0.3f));
        AddDotDamageActorArray(flameEffect);

        break;
    }
    }
}

void ADotDamageManager::AddDotDamageActorArray(ADotDamageActor* _actor)
{
    dotDamageActorArray.Add(_actor);
}

void ADotDamageManager::AddProjectileArray(AMonsterProjectile* _actor)
{
    projectileArray.Add(_actor);
}

void ADotDamageManager::AddElementalOrbArray(AMonsterElementalOrb* _actor)
{
    elementalOrbArray.Add(_actor);
}

void ADotDamageManager::AllDotDamageActorDestroy()
{
    // 현재 생성된 DotDamageActor를 모두 제거
    for (auto actor : dotDamageActorArray)
    {
        if (actor)
            actor->DotDamageActorDestroy();
    }
}

void ADotDamageManager::AllProjectileDestroy()
{
    // 현재 생성된 Projectile을 모두 제거
    for (auto actor : projectileArray)
    {
        if (actor)
            actor->ProjectileDestroy();
    }
}

void ADotDamageManager::AllElementalOrbDestroy()
{
    // 현재 생성된 ElementalOrb를 모두 제거
    for (auto actor : elementalOrbArray)
    {
        if (actor)
            actor->ElementalOrbDestroy();
    }
}

void ADotDamageManager::AllActorDestroy()
{
    AllDotDamageActorDestroy();
    AllProjectileDestroy();
    AllElementalOrbDestroy();
}

void ADotDamageManager::PersonalActorDestroy(AMonsterBase* _monsterBase)
{
    TArray<AActor*> attachedActors;
    _monsterBase->GetAttachedActors(attachedActors);
    for (auto* attached : attachedActors)
    {
        if (attached->ActorHasTag(FName("DotDamage")))
        {
            ADotDamageActor* actor = Cast<ADotDamageActor>(attached);
            actor->DotDamageActorDestroy();
        }
    }
}