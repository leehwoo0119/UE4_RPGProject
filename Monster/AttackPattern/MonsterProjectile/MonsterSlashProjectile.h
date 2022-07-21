// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterProjectile.h"
#include "MonsterSlashProjectile.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectGameInstance;
class UNiagaraComponent;

UCLASS()
class LSH_API AMonsterSlashProjectile : public AMonsterProjectile
{
	GENERATED_BODY()

public:
	AMonsterSlashProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* damageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* slashParticle;


public:
	UFUNCTION()
	virtual void ProjectileDamageCollisionOnOverlapBegin(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);
};
