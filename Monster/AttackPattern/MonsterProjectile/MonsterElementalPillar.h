// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EAIState.h"
#include "MonsterElementalPillar.generated.h"

class USceneComponent;
class UParticleSystemComponent;
class UBoxComponent;
class AMonsterBase;
class AMainChar;
class ADotDamageManager;

UCLASS()
class LSH_API AMonsterElementalPillar : public AActor
{
	GENERATED_BODY()

public:
	AMonsterElementalPillar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* particleSystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* damageCollision;

	AMonsterBase* monsterBase;

	AMainChar* mainChar;

	ADotDamageManager* dotDamageManager;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ElementalType", meta = (AllowPrivateAccess = "true"))
	EAI_ElementalType_State elementalType;

	FTimerHandle lifeTimeHandle;

public:
	UFUNCTION()
	virtual void PillarCollisionOnOverlapBegin(
		UPrimitiveComponent* _overlappedComponent,
		AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex,
		bool _bFromSweep, const FHitResult& _sweepResult);

	void PillarDestroy();
};