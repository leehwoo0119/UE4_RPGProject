// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterElementalProjectile.h"
#include "Components/SphereComponent.h"
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
#include "DotDamageManager.h"
#include "LHW/CharStatusComponent.h"

AMonsterElementalProjectile::AMonsterElementalProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsTargetHoming = false;

	projectileMovement->InitialSpeed = 600.0f;
	projectileMovement->MaxSpeed = 600.0f;

	damageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("damageCollision"));
	damageCollision->SetupAttachment(RootComponent);
}

void AMonsterElementalProjectile::BeginPlay()
{
	Super::BeginPlay();

	damageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonsterElementalProjectile::ProjectileDamageCollisionOnOverlapBegin);
}

void AMonsterElementalProjectile::ProjectileDamageCollisionOnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
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
			if (hitMainChar->GetCharCombatComponent()->CheckInvincibility()) return;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = hitMainChar;
			FVector spawnActorLocation = hitMainChar->GetActorLocation();
			FRotator spawnActorRotator = hitMainChar->GetActorRotation();

			UGameplayStatics::ApplyDamage(hitMainChar, 10.0f, monsterBase->MON_CTRL, this, nullptr);

			if (!hitMainChar->GetCharCombatComponent()->CheckInvincibility() &&
				!hitMainChar->GetCharCombatComponent()->CheckPlayerDefensed())
			{
				float playerMaxHealth = hitMainChar->GetCharStatusComponent()->GetMaxCharHealthPoint();
				dotDamageManager->CreateParticleActor(hitMainChar, playerMaxHealth, spawnActorLocation, spawnActorRotator, monsterBase, elementalType, SpawnParams);
			}
			// 빙결 상태이상은 방어시에도 적용되어야 함
			else if (elementalType == EAI_ElementalType_State::Frozen)
			{
				float playerMaxHealth = hitMainChar->GetCharStatusComponent()->GetMaxCharHealthPoint();
				dotDamageManager->CreateParticleActor(hitMainChar, playerMaxHealth, spawnActorLocation, spawnActorRotator, monsterBase, elementalType, SpawnParams);
			}

			//if (!hitMainChar->GetCharCombatComponent()->CheckInvincibility())
			//{
			//	ProjectileDestroy();
			//}
		}

		if (hitMonsterBase_Normal)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = hitMonsterBase_Normal;
			FVector spawnActorLocation = hitMonsterBase_Normal->GetActorLocation();
			FRotator spawnActorRotator = hitMonsterBase_Normal->GetActorRotation();
			float monsterMaxHealth = hitMonsterBase_Normal->MON_STATUS->GetMaxMonsterHealthPoint();

			UGameplayStatics::ApplyDamage(hitMonsterBase_Normal, 10.0f, monsterBase->MON_CTRL, this, nullptr);
			dotDamageManager->CreateParticleActor(hitMonsterBase_Normal, monsterMaxHealth, spawnActorLocation, spawnActorRotator, monsterBase, elementalType, SpawnParams);	
			//ProjectileDestroy(); 
		}

		ProjectileDestroy();
	}
}

void AMonsterElementalProjectile::Fire(float _delayTime)
{
	projectileMovement->SetVelocityInLocalSpace(FVector(1.0f, 0.0f, 0.0f));

	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
	{
		projectileMovement->SetActive(true);
		projectileMovement->bRotationFollowsVelocity = true;
		ProjectileMove();

	}), _delayTime, false);


	GetWorld()->GetTimerManager().SetTimer(naturalDestroyHandle, FTimerDelegate::CreateLambda([&]()
	{
		ProjectileDestroy();

	}), _delayTime + 10.0f, false);

}