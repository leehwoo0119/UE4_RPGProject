// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterProjectile.generated.h"

class USceneComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class AMonsterBase;
class AMainChar;
class ADotDamageManager;

UCLASS()
class LSH_API AMonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterProjectile();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* projectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* particleSystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* particleTemplate;

	AMonsterBase* monsterBase;

	AMainChar* mainChar;

	ADotDamageManager* dotDamageManager;

	FTimerHandle waitHandle;

	FTimerHandle naturalDestroyHandle;

	FTimerHandle settingWaitHandle;

public:
	UFUNCTION(BlueprintCallable)
	void FindHomingTarget();

	UFUNCTION(BlueprintCallable)
	void ProjectileMove();

	UFUNCTION(BlueprintCallable)
	virtual void Fire(float _delayTime){}

	UFUNCTION(BlueprintCallable)
	void ProjectileDestroy();
};
