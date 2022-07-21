// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterProjectile.h"
#include "EAIState.h"
#include "MonsterElementalProjectile.generated.h"

class USphereComponent;

UCLASS()
class LSH_API AMonsterElementalProjectile : public AMonsterProjectile
{
	GENERATED_BODY()

public:
	AMonsterElementalProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* damageCollision;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsTargetHoming;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ElementalType", meta = (AllowPrivateAccess = "true"))
	EAI_ElementalType_State elementalType;

public:
	UFUNCTION()
	virtual void ProjectileDamageCollisionOnOverlapBegin(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

	void Fire(float _delayTime)override;
};
