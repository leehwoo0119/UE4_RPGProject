// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterElementalPillar.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharCombatComponent.h"
#include "LHW/CharStatusComponent.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "MonsterBase_Normal.h"
#include "MonsterBaseStatus.h"
#include "MonsterProjectile.h"
#include "DotDamageManager.h"

AMonsterElementalPillar::AMonsterElementalPillar()
{
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = rootComp;

	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particleSystemComp"));
	particleSystemComp->SetupAttachment(RootComponent);

	damageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("damageCollision"));
	damageCollision->SetupAttachment(RootComponent);
}

void AMonsterElementalPillar::BeginPlay()
{
	Super::BeginPlay();

	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	dotDamageManager = mainChar->GetDotDamageManager();
	check(dotDamageManager != nullptr);

	GetWorld()->GetTimerManager().SetTimer(lifeTimeHandle, FTimerDelegate::CreateLambda([&]()
	{
		PillarDestroy();
	}), 1.0f, false);

	damageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonsterElementalPillar::PillarCollisionOnOverlapBegin);
}

void AMonsterElementalPillar::PillarCollisionOnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (_otherActor)
	{
		AMainChar* hitMainChar = Cast<AMainChar>(_otherActor);
		AMonsterBase_Normal* hitMonsterBase_Normal = Cast<AMonsterBase_Normal>(_otherActor);

		if (auto projectile = Cast<AMonsterProjectile>(_otherActor)) return;

		if (hitMainChar)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = hitMainChar;
			FVector spawnActorLocation = hitMainChar->GetActorLocation();
			FRotator spawnActorRotator = hitMainChar->GetActorRotation();
			float playerMaxHealth = hitMainChar->GetCharStatusComponent()->GetMaxCharHealthPoint();

			UGameplayStatics::ApplyDamage(hitMainChar, 10.0f, monsterBase->MON_CTRL, this, nullptr);
			dotDamageManager->CreateParticleActor(hitMainChar, playerMaxHealth, spawnActorLocation, spawnActorRotator, monsterBase, elementalType, SpawnParams);
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
		}
	}
}

void AMonsterElementalPillar::PillarDestroy()
{
	GetWorldTimerManager().ClearTimer(lifeTimeHandle);
	Destroy();
}