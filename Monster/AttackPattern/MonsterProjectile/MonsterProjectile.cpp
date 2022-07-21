// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterProjectile.h"
#include "Components/SceneComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "LHW/CharStatusComponent.h"
#include "DotDamageManager.h"

AMonsterProjectile::AMonsterProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = rootComp;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
	projectileMovement->bRotationFollowsVelocity = false;
	projectileMovement->ProjectileGravityScale = 0.0f;

	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particleSystemComp"));
	particleSystemComp->SetupAttachment(RootComponent);

}

void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(particleTemplate != nullptr);

	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);
	
	dotDamageManager = mainChar->GetDotDamageManager();
	check(dotDamageManager != nullptr);

	dotDamageManager->AddProjectileArray(this);
}

void AMonsterProjectile::FindHomingTarget()
{
	TArray<AActor*> findCharArray;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainChar::StaticClass(), findCharArray);
	
	for (AActor* actor : findCharArray)
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), actor->GetActorLocation()));
}

void AMonsterProjectile::ProjectileMove()
{
	projectileMovement->Velocity =
		UKismetMathLibrary::CreateVectorFromYawPitch(GetActorRotation().Yaw, GetActorRotation().Pitch, GetVelocity().Size()) * 1.1f;
}

void AMonsterProjectile::ProjectileDestroy()
{
	GetWorldTimerManager().ClearTimer(waitHandle);
	GetWorldTimerManager().ClearTimer(naturalDestroyHandle);
	GetWorldTimerManager().ClearTimer(settingWaitHandle);
	Destroy();
}

