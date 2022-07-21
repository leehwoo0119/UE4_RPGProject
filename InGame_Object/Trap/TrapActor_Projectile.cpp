// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "LHW/CharStatusComponent.h"
#include "MonsterBaseStatus.h"
#include "MonsterNormalCombatComponent.h"
#include "Particles/ParticleSystemComponent.h"

ATrapActor_Projectile::ATrapActor_Projectile()
{
    projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("projectileMesh"));
    projectileMesh->SetupAttachment(RootComponent);

    arrowTrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("arrowTrailParticle"));
    arrowTrailParticle->SetupAttachment(projectileMesh);

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
    projectileMovement->bRotationFollowsVelocity = false;
    projectileMovement->ProjectileGravityScale = 0.0f;
    projectileMovement->InitialSpeed = 1750.0f;
    projectileMovement->MaxSpeed = 1750.0f;
}

void ATrapActor_Projectile::BeginPlay()
{
    Super::BeginPlay();

    trapCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_Projectile::TrapCollisionOnOverlapBegin);

    spawnLocation = GetActorLocation();
}

void ATrapActor_Projectile::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        AMainChar* mainChar = Cast<AMainChar>(OtherActor);
        AMonsterBase* monsterBase = Cast<AMonsterBase>(OtherActor);

        if (mainChar)
        {
            UGameplayStatics::ApplyDamage(mainChar, mainChar->GetCharStatusComponent()->GetMaxCharHealthPoint() * 0.15f, nullptr, this, nullptr);
        }

        if (monsterBase)
        {
            UMonsterNormalCombatComponent* normalComp = Cast<UMonsterNormalCombatComponent>(monsterBase->MON_COMBATCOMP);
            normalComp->SetInvestigatePosition(spawnLocation);
            UGameplayStatics::ApplyDamage(monsterBase, monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() * 0.15f, nullptr, this, nullptr);
        }

        Destroy();
    }
}