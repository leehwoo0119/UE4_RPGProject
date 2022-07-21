// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterElementalOrb.generated.h"

class AMonsterProjectile;
class UProjectileMovementComponent;
class USceneComponent;
class UParticleSystemComponent;
class AMonsterBase;
class UMonsterNormalCombatComponent;
class AMainChar;
class ADotDamageManager;

UCLASS()
class LSH_API AMonsterElementalOrb : public AActor
{
	GENERATED_BODY()

public:
	AMonsterElementalOrb();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMonsterProjectile> elementalProjectile;

	UPROPERTY()
		int lifeTime;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UProjectileMovementComponent* projectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystemComponent* particleSystemComp;

	UMonsterNormalCombatComponent* monsterNormalCombatComponent;

	AMonsterBase* monsterBase;

	AMainChar* mainChar;

	ADotDamageManager* dotDamageManager;

	FTimerHandle lifeTimeHandle;
	FTimerHandle projectileSpawnHandle;

public:
	UFUNCTION()
		void ProjectileSpawn();

	UFUNCTION()
		void ActorLifeTime();

	UFUNCTION()
		void ElementalOrbDestroy();
};