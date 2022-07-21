// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSlashProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharCombatComponent.h"
#include "MonsterBase.h"
#include "MonsterBase_Normal.h"
#include "MonsterBase_Boss.h"
#include "MonsterBaseController.h"
#include "MonsterBaseStatus.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterWeapon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MonsterBossCombatComponent.h"
#include "MonsterBaseStatus.h"
#include "LHW/CharStatusComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EAIState.h"

AMonsterSlashProjectile::AMonsterSlashProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	projectileMovement->InitialSpeed = 3000.0f;
	projectileMovement->MaxSpeed = 3000.0f;

	damageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("damageCollision"));
	damageCollision->SetupAttachment(RootComponent);

	slashParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("slashParticle"));
	slashParticle->SetupAttachment(RootComponent);
}

void AMonsterSlashProjectile::BeginPlay()
{
	Super::BeginPlay();

	float naturalDestroyTime = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(naturalDestroyHandle, FTimerDelegate::CreateLambda([&]()
	{
		ProjectileDestroy();

	}), naturalDestroyTime, false);

	damageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonsterSlashProjectile::ProjectileDamageCollisionOnOverlapBegin);
}

void AMonsterSlashProjectile::ProjectileDamageCollisionOnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (_otherActor)
	{
		AMainChar* hitMainChar = Cast<AMainChar>(_otherActor);
		AMonsterBase_Normal* hitMonsterBase_Normal = Cast<AMonsterBase_Normal>(_otherActor);

		if (auto projectile = Cast<AMonsterProjectile>(_otherActor)) return;
		if (auto monsterWeapon = Cast<AMonsterWeapon>(_otherActor)) return;
		if (auto hitMonsterBase_Boss = Cast<AMonsterBase_Boss>(_otherActor)) return;

		if (hitMainChar)
		{
			UGameplayStatics::ApplyDamage(hitMainChar, 10.0f, monsterBase->MON_CTRL, this, nullptr);

			if (monsterBase->MON_COMBATCOMP->GetAIMonsterType() == EAI_MonsterType::Boss)
			{
				if (!hitMainChar->GetCharCombatComponent()->CheckInvincibility() &&
					!hitMainChar->GetCharCombatComponent()->CheckPlayerDefensed())
				{
					UMonsterBossCombatComponent* bossCombatComp = Cast<UMonsterBossCombatComponent>(monsterBase->MON_COMBATCOMP);
					bossCombatComp->HealthDrain(monsterBase->MON_STATUS->GetMaxMonsterHealthPoint());
					ProjectileDestroy();
				}
			}
		}

		if (hitMonsterBase_Normal)
		{
			UGameplayStatics::ApplyDamage(hitMonsterBase_Normal, 10.0f, monsterBase->MON_CTRL, this, nullptr);
		}

		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleTemplate, GetActorLocation());
	}
}