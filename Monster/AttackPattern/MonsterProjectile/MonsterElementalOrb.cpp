// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterElementalOrb.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterBase.h"
#include "MonsterStateMachine.h"
#include "LHW/MainChar.h"
#include "DotDamageManager.h"
#include "EAIState.h"
#include "MonsterProjectile.h"
#include "MonsterNormalCombatComponent.h"

AMonsterElementalOrb::AMonsterElementalOrb()
{
    PrimaryActorTick.bCanEverTick = false;

    rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = rootComp;

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
    projectileMovement->InitialSpeed = 500.0f;
    projectileMovement->MaxSpeed = 500.0f;
    projectileMovement->ProjectileGravityScale = 0.0f;
    projectileMovement->Velocity = FVector(0, 0, 1);

    particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particleSystemComp"));
    particleSystemComp->SetupAttachment(RootComponent);

    lifeTime = 15;
}

void AMonsterElementalOrb::BeginPlay()
{
    Super::BeginPlay();

    check(elementalProjectile != nullptr);

    monsterBase = Cast<AMonsterBase>(GetOwner());
    check(monsterBase != nullptr);

    monsterNormalCombatComponent = Cast<UMonsterNormalCombatComponent>(monsterBase->MON_COMBATCOMP);
    check(monsterNormalCombatComponent != nullptr);

    mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    check(mainChar != nullptr);

    dotDamageManager = mainChar->GetDotDamageManager();
    check(dotDamageManager != nullptr);

    dotDamageManager->AddElementalOrbArray(this);

    GetWorld()->GetTimerManager().SetTimer(lifeTimeHandle, this, &AMonsterElementalOrb::ActorLifeTime, 1.0f, true, 1.0f);
    GetWorld()->GetTimerManager().SetTimer(projectileSpawnHandle, this, &AMonsterElementalOrb::ProjectileSpawn, FMath::RandRange(3, 5), true, 1.0f);
}

void AMonsterElementalOrb::ProjectileSpawn()
{
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = monsterBase;

    GetWorld()->SpawnActor<AMonsterProjectile>(elementalProjectile, GetActorLocation(), GetActorRotation(), spawnParams);
}

void AMonsterElementalOrb::ActorLifeTime()
{
    projectileMovement->Velocity = FVector().ZeroVector;
    lifeTime--;
    if (lifeTime <= 0 || monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die)
    {
        ElementalOrbDestroy();
    }
}

void AMonsterElementalOrb::ElementalOrbDestroy()
{
    monsterNormalCombatComponent->PlusCurElementalOrbNum(-1);

    GetWorldTimerManager().ClearTimer(projectileSpawnHandle);
    GetWorldTimerManager().ClearTimer(lifeTimeHandle);
    Destroy();
}